#pragma once

#include "SKCore/SKCoreMinimal.h"

#include <filesystem>

namespace Skylark
{
    struct FSKSampleHostInfo
    {
        FSKString AppName;
        FSKString BackendName;
        std::filesystem::path ScenePath;
        int32 Width = 0;
        int32 Height = 0;
        bool bEnableVSync = true;
    };

    FSKString SKGetSampleSceneDisplayName(const std::filesystem::path& ScenePath);
    FSKString SKBuildSampleWindowTitle(const FSKSampleHostInfo& Info, const FSKString& DynamicSuffix = {});
    void SKPrintSampleLaunchSummary(const FSKSampleHostInfo& Info);
}
