#include "SkSampleCameraOptions.h"

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

        static bool TryParseFloatText(const char* Text, float& OutValue)
        {
            if (!Text || !*Text)
            {
                return false;
            }
            char* EndPtr = nullptr;
            const float Parsed = std::strtof(Text, &EndPtr);
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

            static void ParseScaleEnv(const char* Name, float& InOutValue)
            {
                if (const char* Env = std::getenv(Name))
                {
                    float Parsed = InOutValue;
                    if (TryParseFloatText(Env, Parsed) && Parsed > 0.0f)
                    {
                        InOutValue = Parsed;
                    }
                }
            }
        }

        void SKInitDefaultSampleCameraOptions(FSKSampleCameraOptions& OutOptions)
        {
            OutOptions = {};
            ParseScaleEnv("SKYLARK_ORBIT_SCALE", OutOptions.OrbitScale);
            ParseScaleEnv("SKYLARK_PAN_SCALE", OutOptions.PanScale);
            ParseScaleEnv("SKYLARK_ZOOM_SCALE", OutOptions.ZoomScale);

            if (const char* EnvInvert = std::getenv("SKYLARK_INVERT_ORBIT_Y"))
            {
                bool Parsed = OutOptions.bInvertOrbitY;
                if (TryParseBoolText(EnvInvert, Parsed))
                {
                    OutOptions.bInvertOrbitY = Parsed;
                }
            }

            if (const char* EnvFit = std::getenv("SKYLARK_AUTO_FIT"))
            {
                bool Parsed = OutOptions.bFitOnStartup;
                if (TryParseBoolText(EnvFit, Parsed))
                {
                    OutOptions.bFitOnStartup = Parsed;
                }
            }

            if (const char* EnvControls = std::getenv("SKYLARK_PRINT_CONTROLS"))
            {
                bool Parsed = OutOptions.bPrintControls;
                if (TryParseBoolText(EnvControls, Parsed))
                {
                    OutOptions.bPrintControls = Parsed;
                }
            }
        }

        bool SKParseSampleCameraOptions(int Argc, char** Argv, FSKSampleCameraOptions& OutOptions, FSKString* OutWarnings)
        {
            SKInitDefaultSampleCameraOptions(OutOptions);

            for (int Index = 1; Index < Argc; ++Index)
            {
                const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
                if (Arg.empty())
                {
                    continue;
                }

                auto ParseScale = [&](std::string_view Prefix, float& Target)
                {
                    const std::string Value(Arg.substr(Prefix.size()));
                    float Parsed = Target;
                    if (TryParseFloatText(Value.c_str(), Parsed) && Parsed > 0.0f)
                    {
                        Target = Parsed;
                    }
                    else
                    {
                        AppendWarning(OutWarnings, FSKString("Invalid numeric value for ") + std::string(Prefix).c_str());
                    }
                };

                if (Arg == "--invert-orbit-y")
                {
                    OutOptions.bInvertOrbitY = true;
                    continue;
                }
                if (Arg == "--no-invert-orbit-y")
                {
                    OutOptions.bInvertOrbitY = false;
                    continue;
                }
                if (Arg == "--fit-on-startup")
                {
                    OutOptions.bFitOnStartup = true;
                    continue;
                }
                if (Arg == "--no-fit" || Arg == "--no-fit-on-startup")
                {
                    OutOptions.bFitOnStartup = false;
                    continue;
                }
                if (Arg == "--print-controls")
                {
                    OutOptions.bPrintControls = true;
                    continue;
                }
                if (Arg == "--no-print-controls")
                {
                    OutOptions.bPrintControls = false;
                    continue;
                }

                if (Arg == "--orbit-scale" && (Index + 1) < Argc)
                {
                    float Parsed = OutOptions.OrbitScale;
                    if (TryParseFloatText(Argv[Index + 1], Parsed) && Parsed > 0.0f)
                    {
                        OutOptions.OrbitScale = Parsed;
                    }
                    ++Index;
                    continue;
                }
                if (Arg == "--pan-scale" && (Index + 1) < Argc)
                {
                    float Parsed = OutOptions.PanScale;
                    if (TryParseFloatText(Argv[Index + 1], Parsed) && Parsed > 0.0f)
                    {
                        OutOptions.PanScale = Parsed;
                    }
                    ++Index;
                    continue;
                }
                if (Arg == "--zoom-scale" && (Index + 1) < Argc)
                {
                    float Parsed = OutOptions.ZoomScale;
                    if (TryParseFloatText(Argv[Index + 1], Parsed) && Parsed > 0.0f)
                    {
                        OutOptions.ZoomScale = Parsed;
                    }
                    ++Index;
                    continue;
                }

                if (Arg.rfind("--orbit-scale=", 0) == 0)
                {
                    ParseScale("--orbit-scale=", OutOptions.OrbitScale);
                    continue;
                }
                if (Arg.rfind("--pan-scale=", 0) == 0)
                {
                    ParseScale("--pan-scale=", OutOptions.PanScale);
                    continue;
                }
                if (Arg.rfind("--zoom-scale=", 0) == 0)
                {
                    ParseScale("--zoom-scale=", OutOptions.ZoomScale);
                    continue;
                }
            }

            OutOptions.OrbitScale = std::clamp(OutOptions.OrbitScale, 0.05f, 20.0f);
            OutOptions.PanScale = std::clamp(OutOptions.PanScale, 0.05f, 20.0f);
            OutOptions.ZoomScale = std::clamp(OutOptions.ZoomScale, 0.05f, 20.0f);
            return true;
        }

    void SKPrintSampleCameraOptionSummary(const FSKSampleCameraOptions& Options)
    {
        std::fprintf(stdout,
            "Camera    : orbit-scale=%.3f pan-scale=%.3f zoom-scale=%.3f invert-orbit-y=%s auto-fit=%s controls=%s\n",
            static_cast<double>(Options.OrbitScale),
            static_cast<double>(Options.PanScale),
            static_cast<double>(Options.ZoomScale),
            Options.bInvertOrbitY ? "on" : "off",
            Options.bFitOnStartup ? "on" : "off",
            Options.bPrintControls ? "on" : "off");
    }
}
