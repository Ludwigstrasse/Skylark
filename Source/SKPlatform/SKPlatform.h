#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	/**
	 * 平台层（对标 UE: Core/GenericPlatform + Platform）
	 * - 目标：Skylark 作为嵌入式工业渲染引擎，必须与宿主 UI（Qt/Win32/macOS/Linux）解耦
	 * - 约束：不在头文件直接 include <Windows.h> / <X11/...>，避免污染全局宏
	 */
	struct FSKNativeWindow
	{
		void* Handle = nullptr; // HWND / NSView* / xcb_window_t / etc.
	};

	enum class ESKPlatform : uint8
	{
		Windows = 0,
		Mac,
		Linux,
		Unknown
	};

	class ISKPlatformMisc
	{
	public:
		virtual ~ISKPlatformMisc() = default;
		virtual ESKPlatform GetPlatform() const = 0;
		virtual uint32 GetCoreCount() const = 0;
		virtual FSKString GetUserTempDir() const = 0;
	};

	ISKPlatformMisc& SKGetPlatformMisc();
}
