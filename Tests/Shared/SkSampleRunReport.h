#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SkSampleRuntimeStats.h"
#include "SkSampleBenchmarkOptions.h"
#include "SkCadSceneImporter.h"

#include <filesystem>

namespace Skylark
{
    struct FSKSampleRunReportOptions
    {
        FSKString ReportPath;
        bool bAppend = false;
        bool bPrintPathOnWrite = true;
    };

    struct FSKSampleRunReportInput
    {
        FSKString AppName;
        FSKString BackendName;
        std::filesystem::path ScenePath;
        int32 Width = 0;
        int32 Height = 0;
        bool bEnableVSync = true;
        const FSKSampleRuntimeStatsState* RuntimeStats = nullptr;
        const FSKSampleBenchmarkState* BenchmarkState = nullptr;
        const CadSceneTest::FSkCadSceneDocument* SceneDocument = nullptr;
    };

    void SKInitSampleRunReportOptions(FSKSampleRunReportOptions& OutOptions);
    bool SKParseSampleRunReportOptions(int Argc, char** Argv, FSKSampleRunReportOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleRunReportOptionSummary(const FSKSampleRunReportOptions& Options);
    FSKString SKBuildSampleRunReportText(const FSKSampleRunReportInput& Input);
    bool SKWriteSampleRunReport(
        const FSKSampleRunReportOptions& Options,
        const FSKSampleRunReportInput& Input,
        FSKString* OutError = nullptr,
        std::filesystem::path* OutWrittenPath = nullptr);
}
