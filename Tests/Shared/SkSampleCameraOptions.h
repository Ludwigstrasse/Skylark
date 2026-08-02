#pragma once

#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
    struct FSKSampleCameraOptions
    {
        float OrbitScale = 1.0f;
        float PanScale = 1.0f;
        float ZoomScale = 1.0f;
        bool bInvertOrbitY = false;
        bool bFitOnStartup = true;
        bool bPrintControls = true;
    };

    void SKInitDefaultSampleCameraOptions(FSKSampleCameraOptions& OutOptions);
    bool SKParseSampleCameraOptions(int Argc, char** Argv, FSKSampleCameraOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintSampleCameraOptionSummary(const FSKSampleCameraOptions& Options);
}
