#include "SkSampleAppOptions.h"

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

            static void ParseSizeValue(const char* Text, int32& OutWidth, int32& OutHeight)
            {
                if (!Text)
                {
                    return;
                }
                const std::string Value(Text);
                const size_t Split = Value.find_first_of("xX");
                if (Split == std::string::npos)
                {
                    return;
                }

                int32 ParsedWidth = OutWidth;
                int32 ParsedHeight = OutHeight;
                if (TryParseIntText(Value.substr(0, Split).c_str(), ParsedWidth) &&
                    TryParseIntText(Value.substr(Split + 1).c_str(), ParsedHeight))
                {
                    if (ParsedWidth > 0)
                    {
                        OutWidth = ParsedWidth;
                    }
                    if (ParsedHeight > 0)
                    {
                        OutHeight = ParsedHeight;
                    }
                }
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

        void SKInitDefaultSampleAppOptions(FSKSampleAppOptions& OutOptions)
        {
            OutOptions = {};
            if (const char* EnvScene = std::getenv("SKYLARK_SCENE"))
            {
                OutOptions.ScenePath = EnvScene;
            }
            if (const char* EnvWidth = std::getenv("SKYLARK_WIDTH"))
            {
                int32 Parsed = OutOptions.Width;
                if (TryParseIntText(EnvWidth, Parsed) && Parsed > 0)
                {
                    OutOptions.Width = Parsed;
                }
            }
            if (const char* EnvHeight = std::getenv("SKYLARK_HEIGHT"))
            {
                int32 Parsed = OutOptions.Height;
                if (TryParseIntText(EnvHeight, Parsed) && Parsed > 0)
                {
                    OutOptions.Height = Parsed;
                }
            }
            if (const char* EnvSize = std::getenv("SKYLARK_SIZE"))
            {
                ParseSizeValue(EnvSize, OutOptions.Width, OutOptions.Height);
            }
            if (const char* EnvVsync = std::getenv("SKYLARK_VSYNC"))
            {
                bool Parsed = OutOptions.bEnableVSync;
                if (TryParseBoolText(EnvVsync, Parsed))
                {
                    OutOptions.bEnableVSync = Parsed;
                }
            }
        }

        bool SKParseBasicSampleAppOptions(int Argc, char** Argv, FSKSampleAppOptions& OutOptions, FSKString* OutWarnings)
        {
            SKInitDefaultSampleAppOptions(OutOptions);

            for (int Index = 1; Index < Argc; ++Index)
            {
                const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
                if (Arg.empty())
                {
                    continue;
                }

            if (Arg == "--help" || Arg == "-h")
            {
                OutOptions.bShowHelp = true;
                continue;
            }
            if (Arg.starts_with("--scene="))
            {
                OutOptions.ScenePath = std::string(Arg.substr(8));
                continue;
            }
            if (Arg == "--scene" && (Index + 1) < Argc)
            {
                OutOptions.ScenePath = Argv[++Index];
                continue;
            }
            if (Arg.starts_with("--width="))
            {
                int32 Parsed = OutOptions.Width;
                if (TryParseIntText(std::string(Arg.substr(8)).c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.Width = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --width");
                }
                continue;
            }
            if (Arg == "--width" && (Index + 1) < Argc)
            {
                int32 Parsed = OutOptions.Width;
                if (TryParseIntText(Argv[++Index], Parsed) && Parsed > 0)
                {
                    OutOptions.Width = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --width");
                }
                continue;
            }
            if (Arg.starts_with("--height="))
            {
                int32 Parsed = OutOptions.Height;
                if (TryParseIntText(std::string(Arg.substr(9)).c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.Height = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --height");
                }
                continue;
            }
            if (Arg == "--height" && (Index + 1) < Argc)
            {
                int32 Parsed = OutOptions.Height;
                if (TryParseIntText(Argv[++Index], Parsed) && Parsed > 0)
                {
                    OutOptions.Height = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --height");
                }
                continue;
            }
            if (Arg.starts_with("--size="))
            {
                ParseSizeValue(std::string(Arg.substr(7)).c_str(), OutOptions.Width, OutOptions.Height);
                continue;
            }
            if (Arg == "--size" && (Index + 1) < Argc)
            {
                ParseSizeValue(Argv[++Index], OutOptions.Width, OutOptions.Height);
                continue;
            }
            if (Arg.starts_with("--vsync="))
            {
                bool Parsed = OutOptions.bEnableVSync;
                if (TryParseBoolText(std::string(Arg.substr(8)).c_str(), Parsed))
                {
                    OutOptions.bEnableVSync = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --vsync");
                }
                continue;
            }
            if (Arg == "--vsync" && (Index + 1) < Argc)
            {
                bool Parsed = OutOptions.bEnableVSync;
                if (TryParseBoolText(Argv[++Index], Parsed))
                {
                    OutOptions.bEnableVSync = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, "Invalid value for --vsync");
                }
                continue;
            }

            if (!Arg.starts_with("--") && OutOptions.ScenePath.empty())
            {
                OutOptions.ScenePath = std::string(Arg);
            }
        }
        return true;
    }

        std::filesystem::path SKResolveSampleScenePath(
            const FSKString& ExplicitScenePath,
            std::initializer_list<std::filesystem::path> CandidatePaths,
            const std::filesystem::path& FallbackPath)
        {
            if (!ExplicitScenePath.empty())
            {
                return std::filesystem::path(ExplicitScenePath.c_str());
            }

            for (const auto& Candidate : CandidatePaths)
            {
                if (std::filesystem::exists(Candidate))
                {
                    return std::filesystem::absolute(Candidate);
                }
            }

            return FallbackPath;
        }

    void SKPrintBasicSampleAppHelp(
        const char* AppName,
        const char* BackendDescription,
        const char* ExtraLines,
        const char* EnvironmentLines)
    {
        std::fprintf(stdout,
            "%s options:\n"
            "  --scene=<path-to-csf-or-csf.gz>\n"
            "  --width=<pixels>\n"
            "  --height=<pixels>\n"
            "  --size=<width>x<height>\n"
            "  --vsync=<0|1|on|off>\n"
            "  --help\n"
            "%s%s"
            "\n"
            "Backend: %s\n"
            "%s%s",
            AppName ? AppName : "SkylarkSample",
            ExtraLines ? "" : "",
            ExtraLines ? ExtraLines : "",
            BackendDescription ? BackendDescription : "",
            EnvironmentLines ? "\nEnvironment fallback:\n" : "",
            EnvironmentLines ? EnvironmentLines : "");
    }
}
