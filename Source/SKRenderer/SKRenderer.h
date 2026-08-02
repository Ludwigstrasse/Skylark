#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
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



// ---- Selection / Highlight Inputs (P3) ----
	// User-facing selection filter/highlight mask. Body maps to Object-level selection in CAD apps.
	enum ESKSelectionEntityMask : uint32
	{
		SK_Select_None       = 0u,
		SK_Select_Object     = 1u << 0u,
		SK_Select_Face       = 1u << 1u,
		SK_Select_Edge       = 1u << 2u,
		SK_Select_Vertex     = 1u << 3u,
		SK_Select_BimElement = 1u << 4u,
		SK_Select_All        = 0xFFFFFFFFu,
	};

	SK_FORCEINLINE uint32 SKSelectionMaskFromObjectType(ESKObjectEntityType Type)
	{
		switch (Type)
		{
		case ESKObjectEntityType::Face:    return SK_Select_Face;
		case ESKObjectEntityType::Edge:    return SK_Select_Edge;
		case ESKObjectEntityType::Vertex:  return SK_Select_Vertex;
		case ESKObjectEntityType::Element: return SK_Select_BimElement;
		case ESKObjectEntityType::Body:
		case ESKObjectEntityType::Unknown:
		default:                          return SK_Select_Object;
		}
	}

	SK_FORCEINLINE bool SKSelectionMaskMatches(ESKObjectEntityType Type, uint32 Mask)
	{
		return (SKSelectionMaskFromObjectType(Type) & Mask) != 0u;
	}

	struct FSKSelectionStyle
	{
		bool bEnable = true;
		bool bTintFill = true;
		bool bOutline = true;
		bool bShowHover = true;
		bool bShowSelection = true;

		float SelectedFillAlpha = 0.26f;
		float HoverFillAlpha = 0.18f;
		float OutlineWidth = 2.0f;

		// Packed RGBA8 (R in lowest byte).
		uint32 SelectedFillRGBA8 = 0x42F2B347u; // warm amber, alpha overridden by SelectedFillAlpha
		uint32 HoverFillRGBA8 = 0x42FFDA78u;    // lighter amber, alpha overridden by HoverFillAlpha
		uint32 OutlineRGBA8 = 0xFF1E7BFFu;      // blue outline

		// Which entity kinds are eligible for selection-buffer emission and overlay highlight.
		uint32 PickEntityMask = SK_Select_All;
		uint32 HighlightEntityMask = SK_Select_All;
	};

	struct FSKSelectionState
	{
		TArray<FSKObjectId> SelectedTopoIds;
		TArray<FSKObjectId> HoveredTopoIds;
		FSKSelectionStyle Style{};
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
		FSKSelectionState Selection{};
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
