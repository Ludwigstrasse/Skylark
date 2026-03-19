#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * Skylark Engine 外部接口（对标 UE: IEngine + FEngineLoop，但这里是嵌入式库）
	 * 设计目标：
	 * - SolidDesigner(CAD/BIM/CAE) 作为宿主，负责 UI/窗口/事件循环
	 * - SkylarkEngine 只负责：渲染资源、场景、视口、选择/高亮/剖切/后处理等显示能力
	 */

	struct FSKEngineInitParams
	{
		void* NativeWindowHandle = nullptr; // optional main window HWND/NSView/etc.
		int32 BackBufferWidth = 1280;
		int32 BackBufferHeight = 720;
		bool bEnableVSync = true;

		// UE5-aligned: choose RHI backend
		ESKRHIApi RhiApi = ESKRHIApi::Null;
		bool bEnableRHIDebug = false;

		// 未来：GPU/Adapter、线程策略、缓存策略、渲染特性开关等
	};

	class ISKEngine
	{
	public:
		virtual ~ISKEngine() = default;

		virtual bool Init(const FSKEngineInitParams& Params) = 0;
		virtual void Shutdown() = 0;
		virtual void Tick(float DeltaSeconds) = 0;

		// Viewport 生命周期（支持多视图：MDI / 多窗格 / 视图同步）
		virtual ISKViewport* CreateViewport(const FSKViewportDesc& Desc) = 0;
		virtual void DestroyViewport(ISKViewport* Viewport) = 0;
	};

	// C ABI-ish factory for host integration
	ISKEngine* SKCreateEngine();
	void SKDestroyEngine(ISKEngine* Engine);
}
