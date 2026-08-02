#pragma once

#include "SKCore/SKCoreMinimal.h"

#include <filesystem>
#include <initializer_list>

namespace Skylark
{
    struct FSKSampleAppOptions
    {
        int32 Width = 1600;
        int32 Height = 900;
        bool bEnableVSync = true;
        bool bShowHelp = false;
        FSKString ScenePath;
    };

    void SKInitDefaultSampleAppOptions(FSKSampleAppOptions& OutOptions);
    bool SKParseBasicSampleAppOptions(int Argc, char** Argv, FSKSampleAppOptions& OutOptions, FSKString* OutWarnings = nullptr);
    std::filesystem::path SKResolveSampleScenePath(
        const FSKString& ExplicitScenePath,
        std::initializer_list<std::filesystem::path> CandidatePaths,
        const std::filesystem::path& FallbackPath);
    void SKPrintBasicSampleAppHelp(
        const char* AppName,
        const char* BackendDescription,
        const char* ExtraLines = nullptr,
        const char* EnvironmentLines = nullptr);
}
