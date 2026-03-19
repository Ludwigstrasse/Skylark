#pragma once
#include "SKEngine/SKEngine.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRHI/SKRHI.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
	class FSKSceneGraph;

	/**
	 * Skylark Runtime Engine (embedded)
	 * - Host controls windowing/UI/event loop.
	 * - Runtime owns: RHI device, swapchains, per-viewport pipeline dispatch, and (optionally) legacy bridge.
	 */
	class FSKRuntimeEngine final : public ISKEngine
	{
	public:
		FSKRuntimeEngine();
		~FSKRuntimeEngine() override;

		bool Init(const FSKEngineInitParams& Params) override;
		void Shutdown() override;
		void Tick(float DeltaSeconds) override;

		ISKViewport* CreateViewport(const FSKViewportDesc& Desc) override;
		void DestroyViewport(ISKViewport* Viewport) override;

	private:
		class FViewportImpl final : public ISKViewport, public ISKViewportRHI, public ISKViewportView, public ISKViewportSelectionRHI, public ISKViewportSceneView
		{
		public:
			explicit FViewportImpl(const FSKViewportDesc& InDesc, ISKRHIDevice* InDevice, TSharedPtr<FSKSceneGraph> InScene);

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
		FSKEngineInitParams InitParams{};

		TUniquePtr<ISKRHIDevice> RHIDevice;
		TArray<TUniquePtr<FViewportImpl>> Viewports;

		TUniquePtr<ISKRenderPipeline> Pipeline;

		TSharedPtr<FSKSceneGraph> SceneGraph;

		// Legacy bridge (optional)
		TUniquePtr<class FSKLegacyEngine2Context> Legacy;
	};
}
