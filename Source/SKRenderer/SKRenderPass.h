#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRenderGraph/SKRenderGraph.h"

namespace Skylark
{
	/**
	 * Render pass interface (UE: renderer module passes)
	 * - Pass registers work into RenderGraphBuilder.
	 *
	 * V5: Passes may need viewport-local persistent resources (selection buffer, per-view RTs),
	 *     therefore the viewport is passed in.
	 */
	class ISKRenderPass
	{
	public:
		virtual ~ISKRenderPass() = default;
		virtual const char* GetName() const = 0;
		virtual bool IsEnabled(const FSKViewInfo& View) const { (void)View; return true; }

		virtual void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) = 0;
	};
}
