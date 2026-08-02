#pragma once

#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
    struct FSKSampleRuntimeStatsOptions
    {
        bool bShowFpsInTitle = true;
        bool bPrintFrameStats = false;
        double PrintIntervalSeconds = 2.0;
        int32 TitleUpdateIntervalFrames = 15;
    };

    struct FSKSampleRuntimeStatsState
    {
        uint64 TotalFrameCount = 0;
        double TotalSeconds = 0.0;
        uint64 IntervalFrameCount = 0;
        double IntervalSeconds = 0.0;
        double SmoothedFrameSeconds = 0.0;
        double LastIntervalFps = 0.0;
        double LastIntervalFrameMilliseconds = 0.0;
        int32 FramesSinceTitleRefresh = 0;
    };

    void SKInitDefaultSampleRuntimeStatsOptions(FSKSampleRuntimeStatsOptions& OutOptions);
    bool SKParseSampleRuntimeStatsOptions(int Argc, char** Argv, FSKSampleRuntimeStatsOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleRuntimeStatsOptionSummary(const FSKSampleRuntimeStatsOptions& Options);

    void SKResetSampleRuntimeStats(FSKSampleRuntimeStatsState& InOutState);
    void SKUpdateSampleRuntimeStats(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions& Options, double DeltaSeconds);
    bool SKShouldRefreshSampleRuntimeTitle(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions& Options);
    bool SKTryConsumeSampleRuntimeStatsPrint(FSKSampleRuntimeStatsState& InOutState, const FSKSampleRuntimeStatsOptions& Options, FSKString& OutLine);
    FSKString SKBuildSampleRuntimeStatsSuffix(const FSKSampleRuntimeStatsState& State, const FSKSampleRuntimeStatsOptions& Options);
}
