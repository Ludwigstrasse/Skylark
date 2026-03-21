#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKGeometry/SKGeometryRegistry.h"
#include "SKRHI/SKRHI.h"
#include "SKRenderer/SKRenderer.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
	/**
	 * SKLineRender
	 * - UE: line rendering is typically built as debug/overlay primitives and/or a dedicated pass.
	 * - CAD requirement: stable line list rendering for edges/HLR/annotations.
	 *
	 * V7: line list drawing.
	 * V8: honor per-proxy LocalToWorld and instance batches.
	 */

	// Pack RGBA8 in little-endian friendly order (R in lowest byte).
	SK_FORCEINLINE uint32 SKPackRGBA8(uint8 R, uint8 G, uint8 B, uint8 A)
	{
		return (uint32)R | ((uint32)G << 8u) | ((uint32)B << 16u) | ((uint32)A << 24u);
	}

	struct FSKLineBuildInput
	{
		const FSKSceneView* SceneView = nullptr;
		const FSKViewInfo* View = nullptr;
		FSKEdgeRenderSettings EdgeSettings{};
		FSKLineStyle VisibleStyle{};
		FSKLineStyle HiddenStyle{};
	};

	class FSKLineRenderer final
	{
	public:
		// Build and submit edge lines for current scene view.
		static void DrawEdges(ISKRHICommandList& Cmd, const FSKLineBuildInput& In);
		// SelectionIdBuffer: draw edges with per-proxy HitProxyId encoded in RGBA8.
		static void DrawEdgesForHitProxies(ISKRHICommandList& Cmd, const FSKLineBuildInput& In);
		// Utility: world -> clip
		static FSKVector4f TransformToClip(const FSKVector3f& PWorld, const FSKViewMatrices& Matrices);


	private:
		static void BuildEdgeLines(const FSKGeometryData& Geo,
			const FSKMatrix4f& LocalToWorld,
			const FSKEdgeRenderSettings& EdgeSettings,
			const FSKViewMatrices& Matrices,
			uint32 ColorRGBA8,
			TArray<FSKRHILineVertex>& Out);

	};
}
