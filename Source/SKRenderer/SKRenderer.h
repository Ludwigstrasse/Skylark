#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKPlatform/SKPlatform.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * Renderer module (UE5-aligned surface)
	 * - Renderer logic layer, not the RHI backend.
	 * - CAD/BIM/CAE extensions are expressed as passes + view extensions.
	 */

	enum class ESKViewMode : uint8
	{
		Shaded = 0,
		ShadedWithEdges,
		Wireframe,
		HiddenLine,
		XRay,
	};

	struct FSKViewportDesc
	{
		FSKNativeWindow Window{};
		int32 Width = 1280;
		int32 Height = 720;
		bool bVSync = true;
	};

	struct FSKFrameParams
	{
		float DeltaSeconds = 0.0f;
	};

	// ---- View / Feature Inputs (CAD/BIM/CAE) ----
	struct FSKViewMatrices
	{
		FSKMatrix4f View = FSKMatrix4f::Identity();
		FSKMatrix4f Projection = FSKMatrix4f::Identity();
	};

	struct FSKSectionPlane
	{
		// Plane equation: Ax + By + Cz + D = 0
		float A = 0.0f;
		float B = 0.0f;
		float C = 1.0f;
		float D = 0.0f;
		bool bEnabled = false;
	};

	struct FSKRenderFeatureFlags
	{
		bool bBasePass = true;
		bool bEdges = true;
		bool bHiddenLine = false;
		bool bSelection = true;
		bool bSection = false;
		bool bFieldViz = false;
		bool bPostProcess = true;
	};

	struct FSKBimFilter
	{
		// BIM category/discipline filter (placeholder): bitmask or semantic string set.
		uint64 CategoryMask = ~0ull;
	};

	struct FSKFieldViz
	{
		// CAE field visualization placeholder.
		// Future: scalar/vec fields, colormap LUT, min/max, deformation scale...
		float DeformScale = 1.0f;
		float ValueMin = 0.0f;
		float ValueMax = 1.0f;
	};

	struct FSKViewInfo
	{
		FSKViewMatrices Matrices{};
		ESKViewMode ViewMode = ESKViewMode::ShadedWithEdges;

		FSKRenderFeatureFlags Features{};
		TArray<FSKSectionPlane> SectionPlanes;
		FSKBimFilter BimFilter{};
		FSKFieldViz FieldViz{};
	};

	class ISKViewport
	{
	public:
		virtual ~ISKViewport() = default;
		virtual void Resize(int32 NewWidth, int32 NewHeight) = 0;
		virtual void SetViewMode(ESKViewMode InMode) = 0;
	};

	// Optional interface: expose view inputs (matrices, flags, filters)
	class ISKViewportView
	{
	public:
		virtual ~ISKViewportView() = default;
		virtual const FSKViewInfo& GetViewInfo() const = 0;
		virtual FSKViewInfo& GetViewInfoMutable() = 0;
	};

	// Internal: viewport that can expose its swapchain to the renderer
	class ISKViewportRHI
	{
	public:
		virtual ~ISKViewportRHI() = default;
		virtual ISKRHISwapChain* GetSwapChain() const = 0;
		virtual ISKRHIDevice* GetDevice() const = 0;
	};


	// SceneView access (OSG-like: CullVisitor outputs RenderProxies / DrawList)
	struct FSKSceneView;

	class ISKViewportSceneView
	{
	public:
		virtual ~ISKViewportSceneView() = default;
		virtual const FSKSceneView* GetSceneView() const = 0;
	};

	// Selection buffer access (OCCT/UE-like: GPU id buffer picking)
	class ISKViewportSelectionRHI
	{
	public:
		virtual ~ISKViewportSelectionRHI() = default;
		virtual ISKRHITexture2D* GetSelectionTexture() const = 0;
	};


	class ISKRenderPipeline
	{
	public:
		virtual ~ISKRenderPipeline() = default;
		virtual void RenderFrame(ISKViewport& Viewport, const FSKFrameParams& Params) = 0;
	};

	/**
	 * CAD/BIM/CAE render extension points
	 * - UE: ISceneViewExtension
	 * - OSG: StateSet / CullVisitor hooks
	 * - OCCT: AIS / Prs3d / SelectMgr pipelines
	 */
	class ISKViewExtension
	{
	public:
		virtual ~ISKViewExtension() = default;
		virtual void PreRender(ISKViewport& Viewport) { (void)Viewport; }
		virtual void PostRender(ISKViewport& Viewport) { (void)Viewport; }
	};

	// Factory: default pipeline (RenderGraph-based, CAD/BIM/CAE pass scaffolding)
	TUniquePtr<ISKRenderPipeline> SKCreateDefaultPipeline();
}
