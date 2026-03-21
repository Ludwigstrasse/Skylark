#include "SKRenderer/SKDefaultPasses.h"

#include "SKLineRender/SKLineRender.h"
#include "SKFieldViz/SKFieldViz.h"
#include "SKAnnotation/SKAnnotation.h"

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
			ISKViewport* VPPtr = &Viewport;

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
			[SelectionId, VPPtr](const FSKRGPassContext& Ctx)
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

				// V9: draw hit-proxy id edges into selection buffer (line-based id buffer).
				auto* VP_SV = dynamic_cast<ISKViewportSceneView*>(VPPtr);
				const FSKSceneView* SV = VP_SV ? VP_SV->GetSceneView() : nullptr;
				auto* VP_View = dynamic_cast<ISKViewportView*>(VPPtr);
				const FSKViewInfo* VI = VP_View ? &VP_View->GetViewInfo() : nullptr;
				if (SV && VI)
				{
					FSKLineBuildInput Input;
					Input.SceneView = SV;
					Input.View = VI;
					Input.EdgeSettings = VI->EdgeSettings;
					Input.VisibleStyle = VI->VisibleLineStyle;
					Input.HiddenStyle = VI->HiddenLineStyle;
					FSKLineRenderer::DrawEdgesForHitProxies(*Ctx.Cmd, Input);
				}

				Ctx.Cmd->Flush();

				// TODO: draw face/solid hit-proxy ids (requires triangle pipeline)
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
		// Future: blit/tonemap SceneColor -> backbuffer.
		const auto SceneColorDesc = SKMakeViewportTextureDesc(Viewport, ESKRHIFormat::R8G8B8A8_UNORM, (uint32)(SK_Tex_RenderTarget));
		const auto SceneColor = Graph.GetOrCreateTexture("SceneColor", SceneColorDesc);

		// Capture minimal view settings (avoid dangling references).
		const bool bWantEdges = (View.ViewMode == ESKViewMode::ShadedWithEdges) || (View.ViewMode == ESKViewMode::Wireframe) || View.Features.bEdges;
		const bool bWantHidden = (View.ViewMode == ESKViewMode::HiddenLine) || View.Features.bHiddenLine;
		const FSKEdgeRenderSettings EdgeSettings = View.EdgeSettings;
		const FSKLineStyle VisibleStyle = View.VisibleLineStyle;
		const FSKLineStyle HiddenStyle = View.HiddenLineStyle;
		ISKViewport* VPPtr = &Viewport;

		Graph.AddPass(GetName(),
			[SceneColor](FSKRGPassBuilder& B)
			{
				B.ReadTexture(SceneColor);
			},
			[SceneColor, bWantEdges, bWantHidden, EdgeSettings, VisibleStyle, HiddenStyle, VPPtr](const FSKRGPassContext& Ctx)
			{
				(void)SceneColor;
				if (!Ctx.Cmd || !Ctx.SwapChain)
				{
					return;
				}

				// V7: render overlays directly into the swapchain target.
				Ctx.Cmd->SetSwapChainRenderTarget(*Ctx.SwapChain);
				Ctx.Cmd->ClearRenderTarget(SKCreoBg());

				if (!bWantEdges && !bWantHidden)
				{
					Ctx.Cmd->Flush();
					return;
				}

				auto* VP_SV = dynamic_cast<ISKViewportSceneView*>(VPPtr);
				const FSKSceneView* SV = VP_SV ? VP_SV->GetSceneView() : nullptr;
				if (!SV)
				{
					Ctx.Cmd->Flush();
					return;
				}

				FSKLineBuildInput Input;
				Input.SceneView = SV;
				auto* VP_View = dynamic_cast<ISKViewportView*>(VPPtr);
				Input.View = VP_View ? &VP_View->GetViewInfo() : nullptr;
				Input.EdgeSettings = EdgeSettings;
				Input.VisibleStyle = VisibleStyle;
				Input.HiddenStyle = HiddenStyle;

				// HiddenLine currently approximates as edge overlay. Exact HLR plugs later via ISKHLRProvider.
				FSKLineRenderer::DrawEdges(*Ctx.Cmd, Input);
				// V9: CAE FieldViz overlay (deform + legend) (line-based scaffold).
				if (Input.View && Input.SceneView && Input.SceneView->FieldRegistry)
				{
					FSKFieldVizParams FV;
					FV.DeformScale = Input.View->FieldViz.DeformScale;
					FV.Colormap.MinValue = Input.View->FieldViz.ValueMin;
					FV.Colormap.MaxValue = Input.View->FieldViz.ValueMax;
					FSKFieldVizOverlay::Draw(*Ctx.Cmd, *Input.SceneView, *Input.View, Input.SceneView->FieldRegistry, FV);
				}

				// V9: Annotation overlay (leaders/markup) (line-based placeholder).
				if (Input.SceneView && Input.SceneView->AnnotationManager)
				{
					for (const auto& A : Input.SceneView->AnnotationManager->GetAll())
					{
						TArray<FSKRHILineVertex> AnnVerts;
						AnnVerts.reserve(2);
						const auto C0 = FSKLineRenderer::TransformToClip(A.WorldA, Input.View->Matrices);
						const auto C1 = FSKLineRenderer::TransformToClip(A.WorldB, Input.View->Matrices);

						FSKRHILineVertex V0; V0.X=C0.X; V0.Y=C0.Y; V0.Z=C0.Z; V0.W=C0.W; V0.ColorRGBA8=A.Style.ColorRGBA8;
						FSKRHILineVertex V1; V1.X=C1.X; V1.Y=C1.Y; V1.Z=C1.Z; V1.W=C1.W; V1.ColorRGBA8=A.Style.ColorRGBA8;
						AnnVerts.push_back(V0);
						AnnVerts.push_back(V1);

						FSKRHILineDrawParams P; P.bDepthTest=false; P.Width=A.Style.LineWidth;
						Ctx.Cmd->DrawLineList(AnnVerts.data(), (uint32)AnnVerts.size(), P);
					}
				}

			});
	}
}
