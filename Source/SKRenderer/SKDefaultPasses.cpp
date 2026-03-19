#include "SKRenderer/SKDefaultPasses.h"

#include <algorithm>

namespace Skylark
{
	static FSKRHITextureDesc SKMakeViewportTextureDesc(ISKViewport& Viewport, ESKRHIFormat Format, uint32 Flags)
	{
		FSKRHITextureDesc Desc{};
		Desc.Format = Format;
		Desc.Flags = Flags;

		if (auto* VP_RHI = dynamic_cast<ISKViewportRHI*>(&Viewport))
		{
			if (ISKRHISwapChain* SC = VP_RHI->GetSwapChain())
			{
				const auto& SCD = SC->GetDesc();
				Desc.Width = std::max(1u, SCD.Width);
				Desc.Height = std::max(1u, SCD.Height);
				return Desc;
			}
		}

		Desc.Width = 1;
		Desc.Height = 1;
		return Desc;
	}

	static FSKRHIClearColor SKCreoBg()
	{
		FSKRHIClearColor C;
		// Creo-like light gray background
		C.R = 232.0f / 255.0f;
		C.G = 234.0f / 255.0f;
		C.B = 237.0f / 255.0f;
		C.A = 1.0f;
		return C;
	}

	void FSKBasePass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)View;

		// RDG-Min: SceneColor is a frame-local render target.
		const auto SceneColorDesc = SKMakeViewportTextureDesc(Viewport, ESKRHIFormat::R8G8B8A8_UNORM, (uint32)(SK_Tex_RenderTarget));
		const auto SceneColor = Graph.GetOrCreateTexture("SceneColor", SceneColorDesc);

		Graph.AddPass(GetName(),
			[SceneColor](FSKRGPassBuilder& B)
			{
				B.WriteTexture(SceneColor);
			},
			[SceneColor](const FSKRGPassContext& Ctx)
			{
				if (!Ctx.Cmd)
				{
					return;
				}

				ISKRHITexture2D* RT = Ctx.GetTexture(SceneColor);
				if (!RT)
				{
					return;
				}

				Ctx.Cmd->SetRenderTargetTexture(*RT);
				Ctx.Cmd->ClearRenderTarget(SKCreoBg());
				Ctx.Cmd->Flush();
			});
	}

	void FSKEdgePass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)View;

		const auto SceneColorDesc = SKMakeViewportTextureDesc(Viewport, ESKRHIFormat::R8G8B8A8_UNORM, (uint32)(SK_Tex_RenderTarget));
		const auto SceneColor = Graph.GetOrCreateTexture("SceneColor", SceneColorDesc);

		Graph.AddPass(GetName(),
			[SceneColor](FSKRGPassBuilder& B)
			{
				B.ReadTexture(SceneColor);
				B.WriteTexture(SceneColor); // in-place edge overlay (future)
			},
			[](const FSKRGPassContext& Ctx)
			{
				(void)Ctx;
				// TODO(CAD): silhouette/feature edges, crease angle, screen-space edge AA
			});
	}

	void FSKHiddenLinePass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)Viewport;
		(void)View;

		Graph.AddPass(GetName(),
			[](FSKRGPassBuilder& B) { (void)B; },
			[](const FSKRGPassContext& Ctx)
			{
				(void)Ctx;
				// TODO(CAD): HLR/HiddenLine (hybrid: depth peel + edge extraction)
			});
	}

	void FSKSectionPass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)Viewport;
		(void)View;

		Graph.AddPass(GetName(),
			[](FSKRGPassBuilder& B) { (void)B; },
			[](const FSKRGPassContext& Ctx)
			{
				(void)Ctx;
				// TODO(CAD/BIM): clip planes, section cap rendering, hatch patterns
			});
	}

	void FSKSelectionPass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)View;

		// Selection buffer is viewport-persistent (readback on demand).
		auto* VP_Sel = dynamic_cast<ISKViewportSelectionRHI*>(&Viewport);
		if (!VP_Sel)
		{
			return;
		}
		ISKRHITexture2D* SelTex = VP_Sel->GetSelectionTexture();
		if (!SelTex)
		{
			return;
		}

		const auto SelectionId = Graph.RegisterExternalTexture("SelectionId", *SelTex);

		Graph.AddPass(GetName(),
			[SelectionId](FSKRGPassBuilder& B)
			{
				B.WriteTexture(SelectionId);
			},
			[SelectionId](const FSKRGPassContext& Ctx)
			{
				if (!Ctx.Cmd)
				{
					return;
				}

				ISKRHITexture2D* RT = Ctx.GetTexture(SelectionId);
				if (!RT)
				{
					return;
				}

				Ctx.Cmd->SetRenderTargetTexture(*RT);

				FSKRHIClearColor Clear;
				Clear.R = 0.0f; Clear.G = 0.0f; Clear.B = 0.0f; Clear.A = 0.0f;
				Ctx.Cmd->ClearRenderTarget(Clear);
				Ctx.Cmd->Flush();

				// TODO: draw hit-proxy ids for visible proxies (TopoId -> HitProxyId -> RGBA encoding)
			});
	}

	void FSKFieldVizPass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)Viewport;
		(void)View;

		Graph.AddPass(GetName(),
			[](FSKRGPassBuilder& B) { (void)B; },
			[](const FSKRGPassContext& Ctx)
			{
				(void)Ctx;
				// TODO(CAE): scalar field colormap, deform, glyphs, isolines
			});
	}

	void FSKPostProcessPass::AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View)
	{
		(void)View;

		const auto SceneColorDesc = SKMakeViewportTextureDesc(Viewport, ESKRHIFormat::R8G8B8A8_UNORM, (uint32)(SK_Tex_RenderTarget));
		const auto SceneColor = Graph.GetOrCreateTexture("SceneColor", SceneColorDesc);

		Graph.AddPass(GetName(),
			[SceneColor](FSKRGPassBuilder& B)
			{
				B.ReadTexture(SceneColor);
			},
			[](const FSKRGPassContext& Ctx)
			{
				// TODO: blit SceneColor -> SwapChain backbuffer, then overlays/tonemap.
				if (!Ctx.Cmd || !Ctx.SwapChain)
				{
					return;
				}

				// Placeholder: clear swapchain (keeps pipeline stable while RDG grows).
				Ctx.Cmd->SetSwapChainRenderTarget(*Ctx.SwapChain);
				Ctx.Cmd->ClearRenderTarget(SKCreoBg());
				Ctx.Cmd->Flush();
			});
	}
}
