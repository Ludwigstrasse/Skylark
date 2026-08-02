#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRenderer/SKRenderer.h"
#include "SKScene/SKSceneView.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	struct FSKTriangleSceneBuildInput
	{
		const FSKSceneView* SceneView = nullptr;
		const FSKViewInfo* View = nullptr;
	};

	class FSKTriangleSceneRenderer final
	{
	public:
		static void DrawSolidScene(ISKRHICommandList& Cmd, const FSKTriangleSceneBuildInput& In);
		static void DrawSelectionScene(ISKRHICommandList& Cmd, const FSKTriangleSceneBuildInput& In);
	};
}
