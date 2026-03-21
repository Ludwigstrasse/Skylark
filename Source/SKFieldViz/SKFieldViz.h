#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"
#include "SKScene/SKSceneView.h"
#include "SKRenderer/SKRenderer.h"
#include "SKField/SKField.h"

namespace Skylark
{
	/**
	 * SKFieldViz (V9)
	 * - CAE visualization helpers: deform + legend + (future) colormap shading.
	 * - Current renderer capabilities are line-list based, so V9 focuses on:
	 *   - Deformed edge overlay (vector displacement field)
	 *   - Legend scaffold (line primitives)
	 */

	struct FSKColormap
	{
		// Placeholder: UE/VTK style colormap LUT parameters
		float MinValue = 0.0f;
		float MaxValue = 1.0f;
		bool bLogScale = false;
	};

	struct FSKLegendDesc
	{
		// Normalized viewport space [0..1] rectangle for legend placement
		float X = 0.02f;
		float Y = 0.02f;
		float W = 0.18f;
		float H = 0.35f;
	};

	struct FSKFieldVizParams
	{
		FSKString ActiveScalarField = "Stress";
		FSKString ActiveDeformField = "Displacement";
		float DeformScale = 1.0f;
		FSKColormap Colormap{};
		FSKLegendDesc Legend{};
	};

	class FSKFieldVizOverlay final
	{
	public:
		static void Draw(ISKRHICommandList& Cmd,
			const FSKSceneView& SceneView,
			const FSKViewInfo& View,
			const FSKFieldRegistry* FieldRegistry,
			const FSKFieldVizParams& Params);

	private:
		static void DrawLegend(ISKRHICommandList& Cmd, const FSKLegendDesc& L, uint32 ColorRGBA8);
	};
}
