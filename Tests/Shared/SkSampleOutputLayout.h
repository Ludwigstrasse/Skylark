#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SkSampleBenchmarkOptions.h"
#include "SkSampleRunReport.h"

#include <filesystem>

namespace Skylark
{
    struct FSKSampleOutputLayoutOptions
    {
        FSKString OutputDirectory;
        FSKString SessionTag;
        bool bAutoReportFiles = false;
    };

    void SKInitSampleOutputLayoutOptions(FSKSampleOutputLayoutOptions& OutOptions);
    bool SKParseSampleOutputLayoutOptions(int Argc, char** Argv, FSKSampleOutputLayoutOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleOutputLayoutOptionSummary(const FSKSampleOutputLayoutOptions& Options);

    std::filesystem::path SKBuildSampleOutputLayoutPath(
        const FSKSampleOutputLayoutOptions& Options,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const char* ArtifactTag,
        const char* Extension);

    bool SKApplySampleOutputLayoutDefaults(
        const FSKSampleOutputLayoutOptions& Options,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        FSKSampleBenchmarkOptions& InOutBenchmarkOptions,
        FSKSampleRunReportOptions& InOutRunReportOptions,
        FSKString* OutWarnings = nullptr);
}
