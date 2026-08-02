#pragma once

#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
    enum class ESKSampleControlProfile : uint8
    {
        CadEditorLab,
        CadSceneTechniques,
    };

    FSKString SKBuildSampleControlsText(ESKSampleControlProfile Profile);
    void SKPrintSampleControls(ESKSampleControlProfile Profile);
}
