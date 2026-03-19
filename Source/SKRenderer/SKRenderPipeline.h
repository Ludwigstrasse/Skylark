#pragma once
#include "SKRenderer/SKRenderer.h"
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

	private:
		TArray<TUniquePtr<ISKRenderPass>> Passes;
		TArray<TSharedPtr<ISKViewExtension>> Extensions;
	};
}
