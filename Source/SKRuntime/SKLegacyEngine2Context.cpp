#include "SKRuntime/SKLegacyEngine2Context.h"
#include "SKCore/SKCoreLog.h"

#ifndef SK_RUNTIME_WITH_LEGACY
	#if defined(_WIN32)
		#define SK_RUNTIME_WITH_LEGACY 1
	#else
		#define SK_RUNTIME_WITH_LEGACY 0
	#endif
#endif

#if SK_RUNTIME_WITH_LEGACY
	// NOTE: legacy headers are Windows-only and heavy
	#include "SKSystem.h"
	#include "SKMath.h"
	#include "SKTimer.h"
	#include "SKSceneManager.h"
	#include "SKWorld.h"
	#include "SKRenderer.h"
	#include "SKDx11Renderer.h"
	#include <windows.h>
#endif

namespace Skylark
{
#if SK_RUNTIME_WITH_LEGACY
	struct FLegacyState
	{
		SKEngine2::SKRenderer* Renderer = nullptr;
		SKEngine2::SKSceneManager* SceneManager = nullptr;
		SKEngine2::SKWorld* World = nullptr;

		bool bCreatedTimer = false;
		bool bCreatedSceneManager = false;
		bool bCreatedWorld = false;
	};

	static FLegacyState* CreateLegacy(const FSKEngineInitParams& Params)
	{
		SKEngine2::SKInitSystem();
		SKEngine2::SKInitMath();

		auto* S = new FLegacyState();

		if (!SKEngine2::SKTimer::ms_pTimer)
		{
			(void)SK_NEW SKEngine2::SKTimer();
			S->bCreatedTimer = true;
		}
		if (!SKEngine2::SKSceneManager::ms_pSceneManager)
		{
			(void)SK_NEW SKEngine2::SKSceneManager();
			S->bCreatedSceneManager = true;
		}
		if (!SKEngine2::SKWorld::ms_pWorld)
		{
			(void)SK_NEW SKEngine2::SKWorld();
			S->bCreatedWorld = true;
		}

		S->SceneManager = SKEngine2::SKSceneManager::ms_pSceneManager;
		S->World = SKEngine2::SKWorld::ms_pWorld;

		const HWND Hwnd = reinterpret_cast<HWND>(Params.NativeWindowHandle);
		if (!Hwnd)
		{
			delete S;
			return nullptr;
		}

		S->Renderer = SK_NEW SKEngine2::SKDX11Renderer(
			Hwnd,
			static_cast<unsigned int>(Params.BackBufferWidth),
			static_cast<unsigned int>(Params.BackBufferHeight),
			true,
			16,
			SKEngine2::SKRenderer::MS_4,
			nullptr,
			0);

		SKEngine2::SKRenderer::ms_pRenderer = S->Renderer;

		return S;
	}

	static void DestroyLegacy(FLegacyState* S)
	{
		if (!S) return;

		if (S->Renderer)
		{
			S->Renderer->DeviceLost();
			SK_DELETE(S->Renderer);
			S->Renderer = nullptr;
		}
		SKEngine2::SKRenderer::ms_pRenderer = nullptr;

		// Destroy singletons only if we created them in this context
		if (S->bCreatedWorld && SKEngine2::SKWorld::ms_pWorld)
		{
			SK_DELETE(SKEngine2::SKWorld::ms_pWorld);
			SKEngine2::SKWorld::ms_pWorld = nullptr;
		}
		if (S->bCreatedSceneManager && SKEngine2::SKSceneManager::ms_pSceneManager)
		{
			SK_DELETE(SKEngine2::SKSceneManager::ms_pSceneManager);
			SKEngine2::SKSceneManager::ms_pSceneManager = nullptr;
		}
		if (S->bCreatedTimer && SKEngine2::SKTimer::ms_pTimer)
		{
			SK_DELETE(SKEngine2::SKTimer::ms_pTimer);
			SKEngine2::SKTimer::ms_pTimer = nullptr;
		}

		delete S;
	}
#endif

	FSKLegacyEngine2Context::FSKLegacyEngine2Context() = default;

	FSKLegacyEngine2Context::~FSKLegacyEngine2Context()
	{
		Shutdown();
	}

	bool FSKLegacyEngine2Context::Init(const FSKEngineInitParams& Params)
	{
#if SK_RUNTIME_WITH_LEGACY
		if (bActive)
		{
			return true;
		}

		// Legacy is used only when host passes a HWND and modern RHI is Null.
		Impl = CreateLegacy(Params);
		bActive = (Impl != nullptr);
		if (bActive)
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Display, "SKRuntime: legacy engine2 context enabled.");
		}
		return bActive;
#else
		(void)Params;
		return false;
#endif
	}

	void FSKLegacyEngine2Context::Shutdown()
	{
#if SK_RUNTIME_WITH_LEGACY
		if (!bActive)
		{
			return;
		}
		DestroyLegacy(reinterpret_cast<FLegacyState*>(Impl));
		Impl = nullptr;
		bActive = false;
#endif
	}

	void FSKLegacyEngine2Context::Tick(float DeltaSeconds)
	{
#if SK_RUNTIME_WITH_LEGACY
		if (!bActive) return;

		auto* S = reinterpret_cast<FLegacyState*>(Impl);
		if (S && S->Renderer && S->SceneManager)
		{
			if (S->Renderer->BeginRendering())
			{
				S->SceneManager->Update(static_cast<double>(DeltaSeconds));
				S->SceneManager->Draw(static_cast<double>(DeltaSeconds));
				S->Renderer->EndRendering();
			}
		}
#else
		(void)DeltaSeconds;
#endif
	}
}
