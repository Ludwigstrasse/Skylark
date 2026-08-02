#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SkSampleRuntimeStats.h"

#include <filesystem>

namespace Skylark
{
    struct FSKSampleBenchmarkOptions
    {
        bool bBenchmarkMode = false;
        int32 WarmupFrames = 30;
        int32 MaxMeasuredFrames = 0;
        double MaxMeasuredSeconds = 0.0;
        bool bPrintSummaryOnExit = true;
        bool bAppendCsv = false;
        FSKString CsvPath;
    };

    struct FSKSampleBenchmarkState
    {
        uint64 TotalFrameCount = 0;
        double TotalSeconds = 0.0;
        uint64 MeasuredFrameCount = 0;
        double MeasuredSeconds = 0.0;
        bool bSummaryPrinted = false;
    };

    void SKInitDefaultSampleBenchmarkOptions(FSKSampleBenchmarkOptions& OutOptions);
    bool SKParseSampleBenchmarkOptions(int Argc, char** Argv, FSKSampleBenchmarkOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleBenchmarkOptionSummary(const FSKSampleBenchmarkOptions& Options);

    void SKResetSampleBenchmarkState(FSKSampleBenchmarkState& InOutState);
    void SKUpdateSampleBenchmarkState(FSKSampleBenchmarkState& InOutState, const FSKSampleBenchmarkOptions& Options, double DeltaSeconds);
    bool SKShouldExitSampleBenchmark(const FSKSampleBenchmarkState& State, const FSKSampleBenchmarkOptions& Options);

    FSKString SKBuildSampleBenchmarkSummaryLine(
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const FSKSampleBenchmarkState& State,
        const FSKSampleRuntimeStatsState& RuntimeStats);

    bool SKWriteSampleBenchmarkSummaryCsv(
        const std::filesystem::path& CsvPath,
        bool bAppend,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const FSKSampleBenchmarkState& State,
        const FSKSampleRuntimeStatsState& RuntimeStats,
        FSKString* OutError = nullptr);
}
