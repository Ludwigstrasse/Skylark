#pragma once

#include "SKRenderer/SKRenderer.h"
#include "SKScene/SKSceneView.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    class FSKSelectionRenderer final
    {
    public:
        static void DrawSelectionIds(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View);
    };
}
