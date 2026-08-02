#pragma once

#include "SKRenderer/SKRenderer.h"
#include "SKScene/SKSceneView.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    class FSKSelectionOverlayRenderer final
    {
    public:
        static void DrawSelectionOverlay(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View);
    };
}
