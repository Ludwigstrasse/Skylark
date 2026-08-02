#include "SkCadSceneDiagnostics.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
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

        static void ParseBoolEnv(const char* Name, bool& InOutValue)
        {
            if (const char* Env = std::getenv(Name))
            {
                bool Parsed = InOutValue;
                if (TryParseBoolText(Env, Parsed))
                {
                    InOutValue = Parsed;
                }
            }
        }
    }

    void SKInitCadSceneDiagnosticsOptions(FSKCadSceneDiagnosticsOptions& OutOptions)
    {
        OutOptions = {};
        ParseBoolEnv("SKYLARK_PRINT_SCENE_SUMMARY", OutOptions.bPrintSceneSummary);
        ParseBoolEnv("SKYLARK_PRINT_SCENE_BOUNDS", OutOptions.bPrintSceneBounds);
    }

    bool SKParseCadSceneDiagnosticsOptions(int Argc, char** Argv, FSKCadSceneDiagnosticsOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitCadSceneDiagnosticsOptions(OutOptions);
        for (int32 Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
            if (Arg == "--print-scene-summary")
            {
                OutOptions.bPrintSceneSummary = true;
            }
            else if (Arg == "--no-scene-summary")
            {
                OutOptions.bPrintSceneSummary = false;
            }
            else if (Arg == "--print-scene-bounds")
            {
                OutOptions.bPrintSceneBounds = true;
            }
            else if (Arg == "--no-scene-bounds")
            {
                OutOptions.bPrintSceneBounds = false;
            }
            else if (Arg.rfind("--print-scene-summary=", 0) == 0)
            {
                bool Parsed = OutOptions.bPrintSceneSummary;
                if (TryParseBoolText(Arg.substr(22).data(), Parsed))
                {
                    OutOptions.bPrintSceneSummary = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Unrecognized value for --print-scene-summary: ") + FSKString(Arg.substr(22)));
                }
            }
            else if (Arg.rfind("--print-scene-bounds=", 0) == 0)
            {
                bool Parsed = OutOptions.bPrintSceneBounds;
                if (TryParseBoolText(Arg.substr(21).data(), Parsed))
                {
                    OutOptions.bPrintSceneBounds = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Unrecognized value for --print-scene-bounds: ") + FSKString(Arg.substr(21)));
                }
            }
        }
        return true;
    }

    void SKPrintCadSceneDiagnosticsOptionSummary(const FSKCadSceneDiagnosticsOptions& Options)
    {
        std::fprintf(stdout, "Scene summary : %s\n", Options.bPrintSceneSummary ? "on" : "off");
        std::fprintf(stdout, "Scene bounds  : %s\n", Options.bPrintSceneBounds ? "on" : "off");
    }

    void SKPrintCadSceneDocumentDiagnostics(const CadSceneTest::FSkCadSceneDocument& Document, const FSKCadSceneDiagnosticsOptions& Options)
    {
        if (!Options.bPrintSceneSummary)
        {
            return;
        }

        std::printf("Materials  : %zu\n", Document.Materials.size());
        std::printf("Geometry   : %zu\n", Document.GeometryKeys.size());
        std::printf("Draw items : %zu\n", Document.DrawItems.size());
        std::printf("Scene radius: %.3f\n", Document.SceneRadius);

        if (Options.bPrintSceneBounds)
        {
            std::printf("Scene min  : (%.3f, %.3f, %.3f)\n", Document.SceneMin.X, Document.SceneMin.Y, Document.SceneMin.Z);
            std::printf("Scene max  : (%.3f, %.3f, %.3f)\n", Document.SceneMax.X, Document.SceneMax.Y, Document.SceneMax.Z);
            std::printf("Scene center: (%.3f, %.3f, %.3f)\n", Document.SceneCenter.X, Document.SceneCenter.Y, Document.SceneCenter.Z);
        }
    }
}
