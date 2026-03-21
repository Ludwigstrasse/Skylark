#pragma once
#include "SKRenderer/SKRenderer.h"
#include "SKProfiler/SKProfiler.h"
#include "SKRenderer/SKRenderPass.h"

namespace Skylark
{
	/**
	 * Default render pipeline (UE5 renderer-path style, pass list + view extensions)
	 */
	class FSKRenderPipeline final : public ISKRenderPipeline
	{
	public:
		FSKRenderPipeline();

		void AddPass(TUniquePtr<ISKRenderPass> Pass);
		void AddViewExtension(TSharedPtr<ISKViewExtension> Extension);

		void RenderFrame(ISKViewport& Viewport, const FSKFrameParams& Params) override;

		// V10: last frame CPU pass timings (from RenderGraph).
		const TArray<FSKPassTiming>& GetLastPassTimings() const { return LastPassTimings; }

	private:
		TArray<TUniquePtr<ISKRenderPass>> Passes;
		TArray<TSharedPtr<ISKViewExtension>> Extensions;
		TArray<FSKPassTiming> LastPassTimings;

	};
}
