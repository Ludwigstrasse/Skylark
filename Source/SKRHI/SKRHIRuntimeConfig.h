#pragma once

#include "SKRHI/SKRHIFactory.h"

namespace Skylark
{
    struct FSKRHIRuntimeOptions
    {
        ESKRHIApi RequestedApi = ESKRHIApi::Null;
        bool bEnableDebugLayer = false;
        bool bUseRenderThread = false;
        bool bEnableVSync = true;
        int32 Width = 1600;
        int32 Height = 1000;
        FSKString ScenePath;
        bool bShowHelp = false;
    };

    void SKInitDefaultRHIRuntimeOptions(FSKRHIRuntimeOptions& OutOptions);
    bool SKParseRHIRuntimeOptions(int Argc, char** Argv, FSKRHIRuntimeOptions& OutOptions, FSKString* OutWarnings = nullptr);
    FSKString SKBuildRHIBackendListString(bool bSupportedOnly = false);
}
