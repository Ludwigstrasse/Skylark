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

	
	struct FSKDebugOptions
	{
		bool bDumpRenderGraphJson = false;
		bool bLogDrawStats = false;
		bool bLogStreamingStats = false;
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

struct FSKEdgeRenderSettings
	{
		// Which edge set to display (derived from mesh adjacency or CAD kernel)
		enum class EDisplay : uint8
		{
			All = 0,
			Boundary,
			Sharp,
		};

		EDisplay Display = EDisplay::Sharp;
		float CreaseAngleDeg = 30.0f;
		bool bOverlay = true; // if false, depth-tested (when depth is available)
	};

	struct FSKLineStyle
	{
		float Width = 1.0f;
		bool bAntiAlias = true;
		uint32 ColorRGBA8 = 0xFF000000u;
		uint32 PatternMask = 0xFFFFFFFFu;
		float PatternScale = 1.0f;
	};

	struct FSKBimFilter
	{
		// BIM semantic filters (IFC/Revit): category/level/system masks.
		uint64 CategoryMask = ~0ull;
		uint64 LevelMask = ~0ull;
		uint64 SystemMask = ~0ull;

		// Per-view visibility mask (multi-viewport isolation; default all).
		uint64 ViewVisibilityMask = ~0ull;
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
		// LWC (V11): view origin in double precision.
		FSKVector3d ViewOrigin = FSKVector3d(0.0, 0.0, 0.0);
		FSKViewMatrices Matrices{};
		ESKViewMode ViewMode = ESKViewMode::ShadedWithEdges;

		FSKRenderFeatureFlags Features{};

		// CAD edge/HLR settings (V7)
		FSKEdgeRenderSettings EdgeSettings{};
		FSKLineStyle VisibleLineStyle{};
		FSKLineStyle HiddenLineStyle{ 1.0f, true, 0x80808080u, 0xF0F0F0F0u, 1.0f };

		TArray<FSKSectionPlane> SectionPlanes;
		FSKBimFilter BimFilter{};
		FSKFieldViz FieldViz{};
		FSKDebugOptions Debug{};
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
