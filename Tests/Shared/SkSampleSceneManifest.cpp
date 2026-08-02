#include "SkSampleSceneManifest.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
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

        static FSKString Trim(const FSKString& InValue)
        {
            size_t Begin = 0;
            while (Begin < InValue.size() && std::isspace(static_cast<unsigned char>(InValue[Begin])))
            {
                ++Begin;
            }
            size_t End = InValue.size();
            while (End > Begin && std::isspace(static_cast<unsigned char>(InValue[End - 1])))
            {
                --End;
            }
            return InValue.substr(Begin, End - Begin);
        }
    }

    void SKInitSampleSceneManifestOptions(FSKSampleSceneManifestOptions& OutOptions)
    {
        OutOptions = {};
        if (const char* EnvManifest = std::getenv("SKYLARK_SCENE_MANIFEST"))
        {
            OutOptions.ManifestPath = EnvManifest;
        }
        if (const char* EnvIndex = std::getenv("SKYLARK_SCENE_INDEX"))
        {
            int32 Parsed = OutOptions.SceneIndex;
            if (TryParseIntText(EnvIndex, Parsed) && Parsed >= 0)
            {
                OutOptions.SceneIndex = Parsed;
            }
        }
        if (const char* EnvList = std::getenv("SKYLARK_LIST_SCENES"))
        {
            bool Parsed = OutOptions.bListScenesOnly;
            if (TryParseBoolText(EnvList, Parsed))
            {
                OutOptions.bListScenesOnly = Parsed;
            }
        }
    }

    bool SKParseSampleSceneManifestOptions(int Argc, char** Argv, FSKSampleSceneManifestOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitSampleSceneManifestOptions(OutOptions);

        for (int Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
            if (Arg.empty())
            {
                continue;
            }

            if (Arg.rfind("--scene-manifest=", 0) == 0)
            {
                OutOptions.ManifestPath = FSKString(Arg.substr(17));
                continue;
            }
            if (Arg == "--scene-manifest" && (Index + 1) < Argc)
            {
                OutOptions.ManifestPath = Argv[++Index];
                continue;
            }
            if (Arg.rfind("--scene-index=", 0) == 0)
            {
                int32 Parsed = OutOptions.SceneIndex;
                if (TryParseIntText(std::string(Arg.substr(14)).c_str(), Parsed) && Parsed >= 0)
                {
                    OutOptions.SceneIndex = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid value for --scene-index: ") + FSKString(Arg.substr(14)));
                }
                continue;
            }
            if (Arg == "--scene-index" && (Index + 1) < Argc)
            {
                int32 Parsed = OutOptions.SceneIndex;
                if (TryParseIntText(Argv[++Index], Parsed) && Parsed >= 0)
                {
                    OutOptions.SceneIndex = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid value for --scene-index"));
                }
                continue;
            }
            if (Arg == "--list-scenes")
            {
                OutOptions.bListScenesOnly = true;
                continue;
            }
            if (Arg == "--no-list-scenes")
            {
                OutOptions.bListScenesOnly = false;
                continue;
            }
        }
        return true;
    }

    void SKPrintSampleSceneManifestOptionSummary(const FSKSampleSceneManifestOptions& Options)
    {
        std::fprintf(stdout,
            "Scene manifest: path=%s scene-index=%d list-only=%s\n",
            Options.ManifestPath.empty() ? "<disabled>" : Options.ManifestPath.c_str(),
            static_cast<int>(Options.SceneIndex),
            Options.bListScenesOnly ? "on" : "off");
    }

    bool SKLoadSampleSceneManifest(
        const FSKSampleSceneManifestOptions& Options,
        std::vector<FSKSampleSceneManifestEntry>& OutEntries,
        FSKString* OutError)
    {
        OutEntries.clear();
        if (Options.ManifestPath.empty())
        {
            return true;
        }

        const std::filesystem::path ManifestPath = std::filesystem::path(Options.ManifestPath.c_str());
        std::ifstream Input(ManifestPath);
        if (!Input.is_open())
        {
            if (OutError)
            {
                *OutError = FSKString("Failed to open scene manifest: ") + Options.ManifestPath;
            }
            return false;
        }

        std::string Line;
        int32 LineNumber = 0;
        while (std::getline(Input, Line))
        {
            ++LineNumber;
            FSKString Value = Trim(Line.c_str());
            if (Value.empty() || Value[0] == '#' || Value[0] == ';')
            {
                continue;
            }

            FSKSampleSceneManifestEntry Entry;
            const size_t SplitPipe = Value.find('|');
            const size_t SplitEq = Value.find('=');
            size_t Split = FSKString::npos;
            if (SplitPipe != FSKString::npos)
            {
                Split = SplitPipe;
            }
            else if (SplitEq != FSKString::npos)
            {
                Split = SplitEq;
            }

            if (Split != FSKString::npos)
            {
                Entry.Label = Trim(Value.substr(0, Split));
                Value = Trim(Value.substr(Split + 1));
            }

            if (Value.empty())
            {
                continue;
            }

            std::filesystem::path ScenePath(Value.c_str());
            if (ScenePath.is_relative())
            {
                ScenePath = ManifestPath.parent_path() / ScenePath;
            }
            Entry.ScenePath = std::filesystem::weakly_canonical(ScenePath);
            if (Entry.Label.empty())
            {
                Entry.Label = Entry.ScenePath.filename().string().c_str();
            }
            OutEntries.push_back(std::move(Entry));
        }

        return true;
    }

    std::filesystem::path SKResolveSampleSceneFromManifest(
        const FSKSampleSceneManifestOptions& Options,
        const std::vector<FSKSampleSceneManifestEntry>& Entries,
        FSKString* OutError)
    {
        if (Entries.empty())
        {
            if (!Options.ManifestPath.empty() && OutError)
            {
                *OutError = FSKString("Scene manifest is empty: ") + Options.ManifestPath;
            }
            return {};
        }

        if (Options.SceneIndex < 0 || static_cast<size_t>(Options.SceneIndex) >= Entries.size())
        {
            if (OutError)
            {
                *OutError = FSKString("Scene index out of range for manifest: ") + std::to_string(Options.SceneIndex);
            }
            return {};
        }

        return Entries[static_cast<size_t>(Options.SceneIndex)].ScenePath;
    }

    void SKPrintSampleSceneManifestEntries(const std::vector<FSKSampleSceneManifestEntry>& Entries)
    {
        std::fprintf(stdout, "Manifest scenes (%zu):\n", Entries.size());
        for (size_t Index = 0; Index < Entries.size(); ++Index)
        {
            std::fprintf(stdout, "  [%zu] %s => %s\n",
                Index,
                Entries[Index].Label.empty() ? "<unnamed>" : Entries[Index].Label.c_str(),
                Entries[Index].ScenePath.string().c_str());
        }
    }
}
