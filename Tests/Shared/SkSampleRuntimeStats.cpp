#include "SkSampleRuntimeStats.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

namespace Skylark
{
    namespace
    {
        static FSKString NormalizeText(const char* Text)
        {
            FSKString Out;
            if (!Text)
            {
                return Out;
            }
            while (*Text)
            {
                Out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(*Text))));
                ++Text;
            }
            return Out;
        }

        static bool TryParseBoolText(const char* Text, bool& OutValue)
        {
            const FSKString Value = NormalizeText(Text);
            if (Value == "1" || Value == "true" || Value == "on" || Value == "yes")
            {
                OutValue = true;
                return true;
            }
            if (Value == "0" || Value == "false" || Value == "off" || Value == "no")
            {
                OutValue = false;
                return true;
            }
            return false;
        }

        static bool TryParseIntText(const char* Text, int32& OutValue)
        {
            if (!Text || !*Text)
            {
                return false;
            }
            char* EndPtr = nullptr;
            const long Parsed = std::strtol(Text, &EndPtr, 10);
            if (!EndPtr || *EndPtr != '\0')
            {
                return false;
            }
            OutValue = static_cast<int32>(Parsed);
            return true;
        }

        static bool TryParseDoubleText(const char* Text, double& OutValue)
        {
            if (!Text || !*Text)
            {
                return false;
            }
            char* EndPtr = nullptr;
            const double Parsed = std::strtod(Text, &EndPtr);
            if (!EndPtr || *EndPtr != '\0')
            {
                return false;
            }
            OutValue = Parsed;
            return true;
        }

        static void AppendWarning(FSKString* OutWarnings, const FSKString& Message)
        {
            if (!OutWarnings)
            {
                return;
            }
            if (!OutWarnings->empty())
            {
                OutWarnings->append("\n");
            }
            OutWarnings->append(Message);
        }
    }

    void SKInitDefaultSampleRuntimeStatsOptions(FSKSampleRuntimeStatsOptions& OutOptions)
    {
        OutOptions = {};

        if (const char* EnvTitle = std::getenv("SKYLARK_TITLE_STATS"))
        {
            bool Parsed = OutOptions.bShowFpsInTitle;
            if (TryParseBoolText(EnvTitle, Parsed))
            {
                OutOptions.bShowFpsInTitle = Parsed;
            }
        }

        if (const char* EnvPrint = std::getenv("SKYLARK_PRINT_STATS"))
        {
            bool Parsed = OutOptions.bPrintFrameStats;
            if (TryParseBoolText(EnvPrint, Parsed))
            {
                OutOptions.bPrintFrameStats = Parsed;
            }
        }

        if (const char* EnvInterval = std::getenv("SKYLARK_STATS_INTERVAL"))
        {
            double Parsed = OutOptions.PrintIntervalSeconds;
            if (TryParseDoubleText(EnvInterval, Parsed) && Parsed > 0.05)
            {
                OutOptions.PrintIntervalSeconds = Parsed;
            }
        }

        if (const char* EnvTitleFrames = std::getenv("SKYLARK_TITLE_STATS_INTERVAL_FRAMES"))
        {
            int32 Parsed = OutOptions.TitleUpdateIntervalFrames;
            if (TryParseIntText(EnvTitleFrames, Parsed) && Parsed > 0)
            {
                OutOptions.TitleUpdateIntervalFrames = Parsed;
            }
        }
    }

    bool SKParseSampleRuntimeStatsOptions(int Argc, char** Argv, FSKSampleRuntimeStatsOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitDefaultSampleRuntimeStatsOptions(OutOptions);

        for (int Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
            if (Arg.empty())
            {
                continue;
            }

            auto ParseIntWithValue = [&](int32& Target)
            {
                if ((Index + 1) >= Argc)
                {
                    return false;
                }
                int32 Parsed = Target;
                if (TryParseIntText(Argv[Index + 1], Parsed) && Parsed > 0)
                {
                    Target = Parsed;
                }
                return true;
            };

            auto ParseDoubleWithValue = [&](double& Target)
            {
                if ((Index + 1) >= Argc)
                {
                    return false;
                }
                double Parsed = Target;
                if (TryParseDoubleText(Argv[Index + 1], Parsed) && Parsed > 0.05)
                {
                    Target = Parsed;
                }
                return true;
            };

            if (Arg == "--title-stats")
            {
                OutOptions.bShowFpsInTitle = true;
                continue;
            }
            if (Arg == "--no-title-stats")
            {
                OutOptions.bShowFpsInTitle = false;
                continue;
            }
            if (Arg == "--print-stats")
            {
                OutOptions.bPrintFrameStats = true;
                continue;
            }
            if (Arg == "--no-print-stats")
            {
                OutOptions.bPrintFrameStats = false;
                continue;
            }
            if (Arg == "--stats-interval" && ParseDoubleWithValue(OutOptions.PrintIntervalSeconds))
            {
                ++Index;
                continue;
            }
            if ((Arg == "--title-stats-every" || Arg == "--title-stats-every-frames") && ParseIntWithValue(OutOptions.TitleUpdateIntervalFrames))
            {
                ++Index;
                continue;
            }

            if (Arg.rfind("--title-stats=", 0) == 0)
            {
                bool Parsed = OutOptions.bShowFpsInTitle;
                if (TryParseBoolText(std::string(Arg.substr(14)).c_str(), Parsed))
                {
                    OutOptions.bShowFpsInTitle = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid bool value for --title-stats: ") + std::string(Arg.substr(14)));
                }
                continue;
            }
            if (Arg.rfind("--print-stats=", 0) == 0)
            {
                bool Parsed = OutOptions.bPrintFrameStats;
                if (TryParseBoolText(std::string(Arg.substr(14)).c_str(), Parsed))
                {
                    OutOptions.bPrintFrameStats = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid bool value for --print-stats: ") + std::string(Arg.substr(14)));
                }
                continue;
            }
            if (Arg.rfind("--stats-interval=", 0) == 0)
            {
                double Parsed = OutOptions.PrintIntervalSeconds;
                if (TryParseDoubleText(std::string(Arg.substr(17)).c_str(), Parsed) && Parsed > 0.05)
                {
                    OutOptions.PrintIntervalSeconds = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid numeric value for --stats-interval: ") + std::string(Arg.substr(17)));
                }
                continue;
            }
            if (Arg.rfind("--title-stats-every=", 0) == 0)
            {
                int32 Parsed = OutOptions.TitleUpdateIntervalFrames;
                if (TryParseIntText(std::string(Arg.substr(20)).c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.TitleUpdateIntervalFrames = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid integer value for --title-stats-every: ") + std::string(Arg.substr(20)));
                }
                continue;
            }
            if (Arg.rfind("--title-stats-every-frames=", 0) == 0)
            {
                int32 Parsed = OutOptions.TitleUpdateIntervalFrames;
                if (TryParseIntText(std::string(Arg.substr(27)).c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.TitleUpdateIntervalFrames = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid integer value for --title-stats-every-frames: ") + std::string(Arg.substr(27)));
                }
                continue;
            }
        }

        OutOptions.PrintIntervalSeconds = std::clamp(OutOptions.PrintIntervalSeconds, 0.05, 60.0);
        OutOptions.TitleUpdateIntervalFrames = std::clamp(OutOptions.TitleUpdateIntervalFrames, 1, 600);
        return true;
    }

    void SKPrintSampleRuntimeStatsOptionSummary(const FSKSampleRuntimeStatsOptions& Options)
    {
        std::fprintf(stdout,
            "Runtime   : title-stats=%s print-stats=%s stats-interval=%.2fs title-refresh=%d frames\\n",
            Options.bShowFpsInTitle ? "on" : "off",
            Options.bPrintFrameStats ? "on" : "off",
            Options.PrintIntervalSeconds,
            static_cast<int>(Options.TitleUpdateIntervalFrames));
    }

    void SKResetSampleRuntimeStats(FSKSampleRuntimeStatsState& InOutState)
    {
        InOutState = {};
    }

    void SKUpdateSampleRuntimeStats(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions&, double DeltaSeconds)
    {
        const double ClampedDelta = std::clamp(DeltaSeconds, 0.0, 1.0);
        ++InOutState.TotalFrameCount;
        ++InOutState.IntervalFrameCount;
        InOutState.TotalSeconds += ClampedDelta;
        InOutState.IntervalSeconds += ClampedDelta;
        ++InOutState.FramesSinceTitleRefresh;

        if (InOutState.SmoothedFrameSeconds <= 1.0e-6)
        {
            InOutState.SmoothedFrameSeconds = ClampedDelta;
        }
        else
        {
            const double Alpha = 0.12;
            InOutState.SmoothedFrameSeconds += (ClampedDelta - InOutState.SmoothedFrameSeconds) * Alpha;
        }
    }

    bool SKShouldRefreshSampleRuntimeTitle(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions& Options)
    {
        if (!Options.bShowFpsInTitle)
        {
            return false;
        }
        if (InOutState.TotalFrameCount <= 1)
        {
            InOutState.FramesSinceTitleRefresh = 0;
            return true;
        }
        if (InOutState.FramesSinceTitleRefresh >= Options.TitleUpdateIntervalFrames)
        {
            InOutState.FramesSinceTitleRefresh = 0;
            return true;
        }
        return false;
    }

    bool SKTryConsumeSampleRuntimeStatsPrint(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions& Options, FSKString& OutLine)
    {
        if (!Options.bPrintFrameStats)
        {
            return false;
        }
        if (InOutState.IntervalSeconds < Options.PrintIntervalSeconds || InOutState.IntervalFrameCount == 0)
        {
            return false;
        }

        InOutState.LastIntervalFps = static_cast<double>(InOutState.IntervalFrameCount) / std::max(1.0e-6, InOutState.IntervalSeconds);
        InOutState.LastIntervalFrameMilliseconds = (InOutState.IntervalSeconds * 1000.0) / static_cast<double>(InOutState.IntervalFrameCount);

        char Buffer[256] = {};
        std::snprintf(Buffer, sizeof(Buffer),
            "Stats     : fps=%.2f frame=%.2fms samples=%llu total=%.2fs",
            InOutState.LastIntervalFps,
            InOutState.LastIntervalFrameMilliseconds,
            static_cast<unsigned long long>(InOutState.IntervalFrameCount),
            InOutState.TotalSeconds);

        OutLine = Buffer;
        InOutState.IntervalFrameCount = 0;
        InOutState.IntervalSeconds = 0.0;
        return true;
    }

    FSKString SKBuildSampleRuntimeStatsSuffix(const FSKSampleRuntimeStatsState& State, const FSKSampleRuntimeStatsOptions& Options)
    {
        if (!Options.bShowFpsInTitle)
        {
            return {};
        }

        const double FrameSeconds = std::max(1.0e-6, State.SmoothedFrameSeconds);
        const double Fps = 1.0 / FrameSeconds;
        char Buffer[128] = {};
        std::snprintf(Buffer, sizeof(Buffer), "fps=%.1f  frame=%.2fms", Fps, FrameSeconds * 1000.0);
        return Buffer;
    }
}
