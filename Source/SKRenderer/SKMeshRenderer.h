#pragma once

#include "SKRenderer/SKRenderer.h"
#include "SKRenderer/SKGeometryResidencyCache.h"
#include "SKScene/SKSceneView.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    class FSKMeshRenderer final
    {
    public:
        static void DrawVisibleMeshes(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View);
        static FSKGeometryResidencyCache& GetResidencyCache();
    };
}
