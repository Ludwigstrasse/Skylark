#include "SKRenderer/SKRenderPipeline.h"
#include "SKProfiler/SKProfiler.h"
#include "SKRenderGraph/SKRenderGraph.h"
#include "SKCore/SKCoreLog.h"

namespace Skylark
{
	FSKRenderPipeline::FSKRenderPipeline() = default;

	void FSKRenderPipeline::AddPass(TUniquePtr<ISKRenderPass> Pass)
	{
		if (Pass)
		{
			Passes.push_back(std::move(Pass));
		}
	}

	void FSKRenderPipeline::AddViewExtension(TSharedPtr<ISKViewExtension> Extension)
	{
		if (Extension)
		{
			Extensions.push_back(std::move(Extension));
		}
	}

	void FSKRenderPipeline::RenderFrame(ISKViewport& Viewport, const FSKFrameParams& Params)
	{
		(void)Params;

		auto* VP_RHI = dynamic_cast<ISKViewportRHI*>(&Viewport);
		if (!VP_RHI)
		{
			return;
		}

		ISKRHIDevice* Device = VP_RHI->GetDevice();
		ISKRHISwapChain* SwapChain = VP_RHI->GetSwapChain();
		if (!Device || !SwapChain)
		{
			return;
		}

		FSKViewInfo DefaultView{};
		const FSKViewInfo* View = &DefaultView;
		if (auto* VP_View = dynamic_cast<ISKViewportView*>(&Viewport))
		{
			View = &VP_View->GetViewInfo();
		}

		for (const auto& Ext : Extensions)
		{
			if (Ext)
			{
				Ext->PreRender(Viewport);
			}
		}

		Device->BeginFrame();

		FSKRenderGraphBuilder Graph;
		Graph.Reset();

		FSKFrameProfiler FrameProf;
		FrameProf.BeginFrame();
		Graph.SetProfiler(&FrameProf);

		for (const auto& P : Passes)
		{
			if (!P) continue;
			if (P->IsEnabled(*View))
			{
				P->AddToGraph(Graph, Viewport, *View);
			}
		}

		Graph.Compile();
		Graph.Execute(*Device, *SwapChain);
		FrameProf.EndFrame();
		LastPassTimings = FrameProf.GetTimings();

		SwapChain->Present();
		Device->EndFrame();

		for (const auto& Ext : Extensions)
		{
			if (Ext)
			{
				Ext->PostRender(Viewport);
			}
		}
	}
}
