#pragma once
#include "SKEngine/SKEngine.h"
#include "SKEngine/SKEngineScene.h"
#include "SKGeometry/SKGeometryRegistry.h"
#include "SKField/SKField.h"
#include "SKAnnotation/SKAnnotation.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRHI/SKRHI.h"
#include "SKScene/SKSceneView.h"
#include "SKBIM/SKBIMVisibility.h"
#include "SKStreaming/SKStreaming.h"
#include "SKRenderThread/SKRenderThread.h"

#include <mutex>

namespace Skylark
{
	class FSKSceneGraph;

	/**
	 * Skylark Runtime Engine (embedded)
	 * - Host controls windowing/UI/event loop.
	 * - Runtime owns: RHI device, swapchains, per-viewport pipeline dispatch, and (optionally) legacy bridge.
	 */
	class FSKRuntimeEngine final : public ISKEngine, public ISKEngineSceneAccess
	{
	public:
		FSKRuntimeEngine();
		~FSKRuntimeEngine() override;

		bool Init(const FSKEngineInitParams& Params) override;
		void Shutdown() override;
		void Tick(float DeltaSeconds) override;
		ESKRHIApi GetCurrentRHIApi() const override;
		bool RecreateRHI(ESKRHIApi NewApi) override;

		ISKViewport* CreateViewport(const FSKViewportDesc& Desc) override;
		void DestroyViewport(ISKViewport* Viewport) override;

		// ISKEngineSceneAccess
		TSharedPtr<FSKSceneGraph> GetSceneGraph() const override { return SceneGraph; }
		FSKGeometryRegistry* GetGeometryRegistry() const override { return const_cast<FSKGeometryRegistry*>(&GeometryRegistry); }

	private:
		class FViewportImpl final : public ISKViewport, public ISKViewportRHI, public ISKViewportView, public ISKViewportSelectionRHI, public ISKViewportSceneView
		{
		public:
			explicit FViewportImpl(const FSKViewportDesc& InDesc, ISKRHIDevice* InDevice, TSharedPtr<FSKSceneGraph> InScene);

			void ReleaseRHIResources();
			void RecreateRHIResources(ISKRHIDevice* InDevice);
			void EnsureSelectionTexture();

			void Resize(int32 NewWidth, int32 NewHeight) override;
			void SetViewMode(ESKViewMode InMode) override
			{
				ViewMode = InMode;
				ViewInfo.ViewMode = InMode;
			}

			const FSKViewInfo& GetViewInfo() const override { return ViewInfo; }
			FSKViewInfo& GetViewInfoMutable() override { return ViewInfo; }

			ISKRHISwapChain* GetSwapChain() const override { return SwapChain.get(); }
			ISKRHIDevice* GetDevice() const override { return Device; }

			ISKRHITexture2D* GetSelectionTexture() const override { return SelectionTexture.get(); }
			const FSKSceneView* GetSceneView() const override { return &SceneView; }

			FSKSceneView& GetSceneViewMutable() { return SceneView; }

			FSKViewportDesc Desc{};
			ESKViewMode ViewMode = ESKViewMode::ShadedWithEdges;
			FSKViewInfo ViewInfo{};

		private:
			ISKRHIDevice* Device = nullptr;
			TUniquePtr<ISKRHISwapChain> SwapChain;
			TUniquePtr<ISKRHITexture2D> SelectionTexture;
			FSKSceneView SceneView{};
			TSharedPtr<FSKSceneGraph> SharedScene;

		};

	private:
		bool CreateAndInitRHI(ESKRHIApi RequestedApi);

	private:
		FSKEngineInitParams InitParams{};

		TUniquePtr<ISKRHIDevice> RHIDevice;
		TArray<TUniquePtr<FViewportImpl>> Viewports;

		TUniquePtr<ISKRenderPipeline> Pipeline;

		TSharedPtr<FSKSceneGraph> SceneGraph;
		FSKGeometryRegistry GeometryRegistry;
		FSKFieldRegistry FieldRegistry;
		FSKAnnotationManager AnnotationManager;

		// BIM visibility controller (Hide/Isolate), consumed by culling.
		TSharedPtr<FSKBimVisibilitySet> BimVisibility;

		// Background streaming queue (IO/parse/CPU build tasks).
		TUniquePtr<FSKStreamingQueue> Streaming;

		// Render thread (V11): enqueue render commands from host thread.
		TUniquePtr<FSKRenderThread> RenderThread;
		mutable std::mutex ViewportMutex;

	};
}
