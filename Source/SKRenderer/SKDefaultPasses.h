#pragma once
#include "SKRenderer/SKRenderPass.h"

namespace Skylark
{
	class FSKBasePass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "BasePass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bBasePass; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};

	class FSKEdgePass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "EdgePass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bEdges || View.ViewMode == ESKViewMode::ShadedWithEdges; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};

	class FSKHiddenLinePass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "HiddenLinePass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bHiddenLine || View.ViewMode == ESKViewMode::HiddenLine; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};

	class FSKSectionPass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "SectionPass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bSection && !View.SectionPlanes.empty(); }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};

	class FSKSelectionPass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "SelectionPass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bSelection; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};

	class FSKFieldVizPass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "FieldVizPass"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bFieldViz; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};


class FSKSelectionOverlayPass final : public ISKRenderPass
{
public:
	const char* GetName() const override { return "SelectionOverlay"; }
	bool IsEnabled(const FSKViewInfo& View) const override
	{
		return View.Selection.Style.bEnable
			&& ((!View.Selection.SelectedTopoIds.empty() && View.Selection.Style.bShowSelection)
				|| (!View.Selection.HoveredTopoIds.empty() && View.Selection.Style.bShowHover));
	}
	void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
};

	class FSKPostProcessPass final : public ISKRenderPass
	{
	public:
		const char* GetName() const override { return "PostProcess"; }
		bool IsEnabled(const FSKViewInfo& View) const override { return View.Features.bPostProcess; }
		void AddToGraph(FSKRenderGraphBuilder& Graph, ISKViewport& Viewport, const FSKViewInfo& View) override;
	};
}
