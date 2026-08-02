#include "SkSampleAppHost.h"

#include <cstdio>

namespace Skylark
{
    FSKString SKGetSampleSceneDisplayName(const std::filesystem::path& ScenePath)
    {
        if (ScenePath.empty())
        {
            return "<unspecified>";
        }

        const std::filesystem::path FileName = ScenePath.filename();
        if (!FileName.empty())
        {
            return FileName.string().c_str();
        }

        return ScenePath.string().c_str();
    }

    FSKString SKBuildSampleWindowTitle(const FSKSampleHostInfo& Info, const FSKString& DynamicSuffix)
    {
        FSKString Title = Info.AppName.empty() ? FSKString("Skylark") : Info.AppName;

        if (!Info.BackendName.empty())
        {
            Title += " [";
            Title += Info.BackendName;
            Title += "]";
        }

        if (!Info.ScenePath.empty())
        {
            Title += " ";
            Title += SKGetSampleSceneDisplayName(Info.ScenePath);
        }

        if (!DynamicSuffix.empty())
        {
            Title += "  ";
            Title += DynamicSuffix;
        }

        return Title;
    }

    void SKPrintSampleLaunchSummary(const FSKSampleHostInfo& Info)
    {
        std::fprintf(stdout, "App       : %s\n", Info.AppName.empty() ? "Skylark" : Info.AppName.c_str());
        std::fprintf(stdout, "Backend   : %s\n", Info.BackendName.empty() ? "<unspecified>" : Info.BackendName.c_str());
        std::fprintf(stdout, "Scene     : %s\n", Info.ScenePath.empty() ? "<unspecified>" : Info.ScenePath.string().c_str());
        std::fprintf(stdout, "Window    : %dx%d\n", static_cast<int>(Info.Width), static_cast<int>(Info.Height));
        std::fprintf(stdout, "VSync     : %s\n", Info.bEnableVSync ? "on" : "off");
    }
}
