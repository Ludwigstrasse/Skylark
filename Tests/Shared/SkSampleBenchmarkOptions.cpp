#include "SkSampleBenchmarkOptions.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
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

        static bool IsBenchmarkEffectivelyEnabled(const FSKSampleBenchmarkOptions& Options)
        {
            return Options.bBenchmarkMode ||
                Options.MaxMeasuredFrames > 0 ||
                Options.MaxMeasuredSeconds > 0.0 ||
                !Options.CsvPath.empty();
        }

        static FSKString EscapeCsvField(const FSKString& InValue)
        {
            FSKString Value = InValue;
            size_t Pos = 0;
            while ((Pos = Value.find('"', Pos)) != FSKString::npos)
            {
                Value.insert(Pos, 1, '"');
                Pos += 2;
            }
            return '"' + Value + '"';
        }
    }

    void SKInitDefaultSampleBenchmarkOptions(FSKSampleBenchmarkOptions& OutOptions)
    {
        OutOptions = {};

        if (const char* EnvBenchmark = std::getenv("SKYLARK_BENCHMARK"))
        {
            bool Parsed = OutOptions.bBenchmarkMode;
            if (TryParseBoolText(EnvBenchmark, Parsed))
            {
                OutOptions.bBenchmarkMode = Parsed;
            }
        }
        if (const char* EnvWarmup = std::getenv("SKYLARK_WARMUP_FRAMES"))
        {
            int32 Parsed = OutOptions.WarmupFrames;
            if (TryParseIntText(EnvWarmup, Parsed) && Parsed >= 0)
            {
                OutOptions.WarmupFrames = Parsed;
            }
        }
        if (const char* EnvMaxFrames = std::getenv("SKYLARK_MAX_FRAMES"))
        {
            int32 Parsed = OutOptions.MaxMeasuredFrames;
            if (TryParseIntText(EnvMaxFrames, Parsed) && Parsed >= 0)
            {
                OutOptions.MaxMeasuredFrames = Parsed;
            }
        }
        if (const char* EnvMaxSeconds = std::getenv("SKYLARK_MAX_SECONDS"))
        {
            double Parsed = OutOptions.MaxMeasuredSeconds;
            if (TryParseDoubleText(EnvMaxSeconds, Parsed) && Parsed >= 0.0)
            {
                OutOptions.MaxMeasuredSeconds = Parsed;
            }
        }
        if (const char* EnvCsv = std::getenv("SKYLARK_BENCHMARK_CSV"))
        {
            OutOptions.CsvPath = EnvCsv;
        }
        if (const char* EnvAppendCsv = std::getenv("SKYLARK_BENCHMARK_APPEND_CSV"))
        {
            bool Parsed = OutOptions.bAppendCsv;
            if (TryParseBoolText(EnvAppendCsv, Parsed))
            {
                OutOptions.bAppendCsv = Parsed;
            }
        }
        if (const char* EnvSummary = std::getenv("SKYLARK_BENCHMARK_SUMMARY"))
        {
            bool Parsed = OutOptions.bPrintSummaryOnExit;
            if (TryParseBoolText(EnvSummary, Parsed))
            {
                OutOptions.bPrintSummaryOnExit = Parsed;
            }
        }

        if (IsBenchmarkEffectivelyEnabled(OutOptions))
        {
            OutOptions.bBenchmarkMode = true;
        }
    }

    bool SKParseSampleBenchmarkOptions(int Argc, char** Argv, FSKSampleBenchmarkOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitDefaultSampleBenchmarkOptions(OutOptions);

        for (int Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? Argv[Index] : "";

            if (Arg == "--benchmark")
            {
                OutOptions.bBenchmarkMode = true;
                continue;
            }
            if (Arg == "--no-benchmark")
            {
                OutOptions.bBenchmarkMode = false;
                continue;
            }
            if (Arg.rfind("--warmup-frames=", 0) == 0)
            {
                int32 Parsed = OutOptions.WarmupFrames;
                if (TryParseIntText(Arg.substr(16).data(), Parsed) && Parsed >= 0)
                {
                    OutOptions.WarmupFrames = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --warmup-frames.");
                }
                continue;
            }
            if (Arg.rfind("--max-frames=", 0) == 0)
            {
                int32 Parsed = OutOptions.MaxMeasuredFrames;
                if (TryParseIntText(Arg.substr(13).data(), Parsed) && Parsed >= 0)
                {
                    OutOptions.MaxMeasuredFrames = Parsed;
                    if (Parsed > 0)
                    {
                        OutOptions.bBenchmarkMode = true;
                    }
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --max-frames.");
                }
                continue;
            }
            if (Arg.rfind("--max-seconds=", 0) == 0)
            {
                double Parsed = OutOptions.MaxMeasuredSeconds;
                if (TryParseDoubleText(Arg.substr(14).data(), Parsed) && Parsed >= 0.0)
                {
                    OutOptions.MaxMeasuredSeconds = Parsed;
                    if (Parsed > 0.0)
                    {
                        OutOptions.bBenchmarkMode = true;
                    }
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --max-seconds.");
                }
                continue;
            }
            if (Arg.rfind("--benchmark-csv=", 0) == 0)
            {
                OutOptions.CsvPath = FSKString(Arg.substr(16));
                if (!OutOptions.CsvPath.empty())
                {
                    OutOptions.bBenchmarkMode = true;
                }
                continue;
            }
            if (Arg == "--append-benchmark-csv")
            {
                OutOptions.bAppendCsv = true;
                continue;
            }
            if (Arg == "--print-benchmark-summary")
            {
                OutOptions.bPrintSummaryOnExit = true;
                continue;
            }
            if (Arg == "--no-benchmark-summary")
            {
                OutOptions.bPrintSummaryOnExit = false;
                continue;
            }
        }

        if (IsBenchmarkEffectivelyEnabled(OutOptions))
        {
            OutOptions.bBenchmarkMode = true;
        }
        return true;
    }

    void SKPrintSampleBenchmarkOptionSummary(const FSKSampleBenchmarkOptions& Options)
    {
        std::fprintf(stdout,
            "Benchmark   : %s  warmup_frames=%d  max_frames=%d  max_seconds=%.3f  csv=%s  append_csv=%s  summary=%s\n",
            Options.bBenchmarkMode ? "on" : "off",
            Options.WarmupFrames,
            Options.MaxMeasuredFrames,
            Options.MaxMeasuredSeconds,
            Options.CsvPath.empty() ? "<none>" : Options.CsvPath.c_str(),
            Options.bAppendCsv ? "on" : "off",
            Options.bPrintSummaryOnExit ? "on" : "off");
    }

    void SKResetSampleBenchmarkState(FSKSampleBenchmarkState& InOutState)
    {
        InOutState = {};
    }

    void SKUpdateSampleBenchmarkState(FSKSampleBenchmarkState& InOutState, const FSKSampleBenchmarkOptions& Options, double DeltaSeconds)
    {
        InOutState.TotalFrameCount += 1;
        InOutState.TotalSeconds += DeltaSeconds;

        if (static_cast<int32>(InOutState.TotalFrameCount) > Options.WarmupFrames)
        {
            InOutState.MeasuredFrameCount += 1;
            InOutState.MeasuredSeconds += DeltaSeconds;
        }
    }

    bool SKShouldExitSampleBenchmark(const FSKSampleBenchmarkState& State, const FSKSampleBenchmarkOptions& Options)
    {
        if (!Options.bBenchmarkMode)
        {
            return false;
        }
        if (Options.MaxMeasuredFrames > 0 && static_cast<int32>(State.MeasuredFrameCount) >= Options.MaxMeasuredFrames)
        {
            return true;
        }
        if (Options.MaxMeasuredSeconds > 0.0 && State.MeasuredSeconds >= Options.MaxMeasuredSeconds)
        {
            return true;
        }
        return false;
    }

    FSKString SKBuildSampleBenchmarkSummaryLine(
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const FSKSampleBenchmarkState& State,
        const FSKSampleRuntimeStatsState& RuntimeStats)
    {
        const double AvgMeasuredFps = State.MeasuredSeconds > 1.0e-9 ? static_cast<double>(State.MeasuredFrameCount) / State.MeasuredSeconds : 0.0;
        const double AvgMeasuredMs = AvgMeasuredFps > 1.0e-9 ? 1000.0 / AvgMeasuredFps : 0.0;

        char Buffer[1024];
        std::snprintf(
            Buffer,
            sizeof(Buffer),
            "Benchmark summary: app=%s backend=%s scene=%s total_frames=%llu total_seconds=%.3f measured_frames=%llu measured_seconds=%.3f avg_fps=%.3f avg_ms=%.3f smoothed_ms=%.3f",
            AppName ? AppName : "<unknown>",
            BackendName ? BackendName : "<unknown>",
            ScenePath.empty() ? "<none>" : ScenePath.string().c_str(),
            static_cast<unsigned long long>(State.TotalFrameCount),
            State.TotalSeconds,
            static_cast<unsigned long long>(State.MeasuredFrameCount),
            State.MeasuredSeconds,
            AvgMeasuredFps,
            AvgMeasuredMs,
            RuntimeStats.SmoothedFrameSeconds * 1000.0);
        return FSKString(Buffer);
    }

    bool SKWriteSampleBenchmarkSummaryCsv(
        const std::filesystem::path& CsvPath,
        bool bAppend,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const FSKSampleBenchmarkState& State,
        const FSKSampleRuntimeStatsState& RuntimeStats,
        FSKString* OutError)
    {
        if (CsvPath.empty())
        {
            return true;
        }

        std::error_code Ec;
        if (!CsvPath.parent_path().empty())
        {
            std::filesystem::create_directories(CsvPath.parent_path(), Ec);
        }

        const bool bAlreadyExists = std::filesystem::exists(CsvPath);
        const char* OpenMode = (bAppend || bAlreadyExists) ? "a" : "w";
        FILE* File = std::fopen(CsvPath.string().c_str(), OpenMode);
        if (!File)
        {
            if (OutError)
            {
                *OutError = FSKString("Failed to open benchmark csv: ") + CsvPath.string().c_str();
            }
            return false;
        }

        if (!bAlreadyExists || (!bAppend && std::filesystem::file_size(CsvPath, Ec) == 0))
        {
            std::fprintf(File, "app,backend,scene,total_frames,total_seconds,measured_frames,measured_seconds,avg_fps,avg_ms,smoothed_ms\n");
        }

        const double AvgMeasuredFps = State.MeasuredSeconds > 1.0e-9 ? static_cast<double>(State.MeasuredFrameCount) / State.MeasuredSeconds : 0.0;
        const double AvgMeasuredMs = AvgMeasuredFps > 1.0e-9 ? 1000.0 / AvgMeasuredFps : 0.0;

        const FSKString App = EscapeCsvField(AppName ? FSKString(AppName) : FSKString("<unknown>"));
        const FSKString Backend = EscapeCsvField(BackendName ? FSKString(BackendName) : FSKString("<unknown>"));
        const FSKString Scene = EscapeCsvField(ScenePath.empty() ? FSKString("<none>") : FSKString(ScenePath.string().c_str()));

        std::fprintf(File,
            "%s,%s,%s,%llu,%.6f,%llu,%.6f,%.6f,%.6f,%.6f\n",
            App.c_str(),
            Backend.c_str(),
            Scene.c_str(),
            static_cast<unsigned long long>(State.TotalFrameCount),
            State.TotalSeconds,
            static_cast<unsigned long long>(State.MeasuredFrameCount),
            State.MeasuredSeconds,
            AvgMeasuredFps,
            AvgMeasuredMs,
            RuntimeStats.SmoothedFrameSeconds * 1000.0);

        std::fclose(File);
        return true;
    }
}
