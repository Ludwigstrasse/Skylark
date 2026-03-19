#include "SKRuntime/SKRuntimeEngine.h"
#include "SKCore/SKCoreLog.h"
#include "SKRHI/SKRHIFactory.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRuntime/SKLegacyEngine2Context.h"
#include "SKScene/SKSceneGraph.h"

#ifndef SK_RUNTIME_WITH_LEGACY
	#if defined(_WIN32)
		#define SK_RUNTIME_WITH_LEGACY 1
	#else
		#define SK_RUNTIME_WITH_LEGACY 0
	#endif
#endif

namespace Skylark
{
	// ---------------------------------------------
	FSKRuntimeEngine::FViewportImpl::FViewportImpl(const FSKViewportDesc& InDesc, ISKRHIDevice* InDevice, TSharedPtr<FSKSceneGraph> InScene)
		: Desc(InDesc)
		, Device(InDevice)
		, SharedScene(std::move(InScene))
	{
		ViewInfo.ViewMode = ViewMode;
		if (Device)
		{
			FSKRHISwapChainDesc SC;
			SC.Window = Desc.Window;
			SC.Width = static_cast<uint32>(Desc.Width);
			SC.Height = static_cast<uint32>(Desc.Height);
			SC.bVSync = Desc.bVSync;
			SwapChain = Device->CreateSwapChain(SC);
		}

		EnsureSelectionTexture();
	}

	void FSKRuntimeEngine::FViewportImpl::EnsureSelectionTexture()
	{
		if (!Device)
		{
			return;
		}

		const uint32 W = (Desc.Width <= 0) ? 1u : static_cast<uint32>(Desc.Width);
		const uint32 H = (Desc.Height <= 0) ? 1u : static_cast<uint32>(Desc.Height);

		if (SelectionTexture)
		{
			const auto& TD = SelectionTexture->GetDesc();
			if (TD.Width == W && TD.Height == H)
			{
				return;
			}
		}

		FSKRHITextureDesc TD{};
		TD.Width = W;
		TD.Height = H;
		TD.Format = ESKRHIFormat::R8G8B8A8_UNORM;
		TD.Flags = (uint32)(SK_Tex_RenderTarget | SK_Tex_CPUReadback);

		SelectionTexture = Device->CreateTexture2D(TD);
	}

	void FSKRuntimeEngine::FViewportImpl::Resize(int32 NewWidth, int32 NewHeight)
	{
		Desc.Width = NewWidth;
		Desc.Height = NewHeight;
		if (SwapChain)
		{
			SwapChain->Resize(static_cast<uint32>(NewWidth), static_cast<uint32>(NewHeight));
		}
		EnsureSelectionTexture();
	}

	// ---------------------------------------------
	FSKRuntimeEngine::FSKRuntimeEngine() = default;

	FSKRuntimeEngine::~FSKRuntimeEngine()
	{
		Shutdown();
	}

	bool FSKRuntimeEngine::Init(const FSKEngineInitParams& Params)
	{
		InitParams = Params;

		FSKRHIDeviceDesc RD;
		RD.Api = Params.RhiApi;
		RD.bEnableDebugLayer = Params.bEnableRHIDebug;

		RHIDevice = SKCreateRHIDevice(RD);
		if (!RHIDevice || !RHIDevice->Init(RD))
		{
			if (RD.Api != ESKRHIApi::Null)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKRuntime: RHI init failed for requested backend. Falling back to NullRHI.");
				RD.Api = ESKRHIApi::Null;
				RHIDevice = SKCreateRHIDevice(RD);
			}
			if (!RHIDevice || !RHIDevice->Init(RD))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "SKRuntime: RHI init failed.");
				return false;
			}
		}

		// SceneGraph (OSG-aligned)
		SceneGraph = std::make_shared<FSKSceneGraph>();
		{
			// Minimal demo content: one geometry node with a stable hit-proxy id.
			auto Geo = std::make_shared<FSKGeometryNode>("DemoGeometry");
			Geo->SetGeometryKey(1);
			Geo->SetHitProxyId(1);
			SceneGraph->GetRoot()->AddChild(Geo);
		}

		Pipeline = SKCreateDefaultPipeline();

#if SK_RUNTIME_WITH_LEGACY
		// Enable legacy ONLY when explicitly requested by host (NativeWindowHandle != nullptr) AND RHI is Null.
		if (Params.NativeWindowHandle && Params.RhiApi == ESKRHIApi::Null)
		{
			Legacy = std::make_unique<FSKLegacyEngine2Context>();
			if (!Legacy->Init(Params))
			{
				Legacy.reset();
			}
		}
#endif

		return true;
	}

	void FSKRuntimeEngine::Shutdown()
	{
		Viewports.clear();
		Pipeline.reset();
		SceneGraph.reset();

#if SK_RUNTIME_WITH_LEGACY
		if (Legacy)
		{
			Legacy->Shutdown();
			Legacy.reset();
		}
#endif

		if (RHIDevice)
		{
			RHIDevice->Shutdown();
			RHIDevice.reset();
		}
	}

	void FSKRuntimeEngine::Tick(float DeltaSeconds)
	{
		FSKFrameParams Frame{};
		Frame.DeltaSeconds = DeltaSeconds;

		for (auto& VP : Viewports)
		{
			if (!VP)
			{
				continue;
			}

			// Update/Cull (OSG-like)
			if (SceneGraph)
			{
				FSKUpdateVisitor Update;
				SceneGraph->Traverse(Update);

				FSKCullVisitor Cull(VP->GetSceneViewMutable());
				SceneGraph->Traverse(Cull);
			}

			if (Pipeline)
			{
				Pipeline->RenderFrame(*VP, Frame);
			}
		}

#if SK_RUNTIME_WITH_LEGACY
		if (Legacy && Legacy->IsActive())
		{
			Legacy->Tick(DeltaSeconds);
		}
#endif
	}

	ISKViewport* FSKRuntimeEngine::CreateViewport(const FSKViewportDesc& Desc)
	{
		Viewports.push_back(std::make_unique<FViewportImpl>(Desc, RHIDevice.get(), SceneGraph));
		return Viewports.back().get();
	}

	void FSKRuntimeEngine::DestroyViewport(ISKViewport* Viewport)
	{
		for (auto It = Viewports.begin(); It != Viewports.end(); ++It)
		{
			if (It->get() == Viewport)
			{
				Viewports.erase(It);
				return;
			}
		}
	}
}
