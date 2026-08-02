#pragma once

#include "SKCore/SKCoreMinimal.h"

#include <filesystem>
#include <vector>

namespace Skylark
{
    struct FSKSampleSceneManifestOptions
    {
        FSKString ManifestPath;
        int32 SceneIndex = 0;
        bool bListScenesOnly = false;
    };

    struct FSKSampleSceneManifestEntry
    {
        FSKString Label;
        std::filesystem::path ScenePath;
    };

    void SKInitSampleSceneManifestOptions(FSKSampleSceneManifestOptions& OutOptions);
    bool SKParseSampleSceneManifestOptions(int Argc, char** Argv, FSKSampleSceneManifestOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleSceneManifestOptionSummary(const FSKSampleSceneManifestOptions& Options);

    bool SKLoadSampleSceneManifest(
        const FSKSampleSceneManifestOptions& Options,
        std::vector<FSKSampleSceneManifestEntry>& OutEntries,
        FSKString* OutError = nullptr);

    std::filesystem::path SKResolveSampleSceneFromManifest(
        const FSKSampleSceneManifestOptions& Options,
        const std::vector<FSKSampleSceneManifestEntry>& Entries,
        FSKString* OutError = nullptr);

    void SKPrintSampleSceneManifestEntries(const std::vector<FSKSampleSceneManifestEntry>& Entries);
}
