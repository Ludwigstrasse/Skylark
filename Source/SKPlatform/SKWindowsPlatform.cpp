#include "SKPlatform/SKWindowsPlatform.h"

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <Windows.h>
#endif

namespace Skylark
{
	uint32 FSKWindowsPlatformMisc::GetCoreCount() const
	{
	#if defined(_WIN32)
		SYSTEM_INFO Info{};
		::GetSystemInfo(&Info);
		return static_cast<uint32>(Info.dwNumberOfProcessors);
	#else
		return 1;
	#endif
	}

	FSKString FSKWindowsPlatformMisc::GetUserTempDir() const
	{
	#if defined(_WIN32)
		wchar_t Buffer[MAX_PATH]{};
		const DWORD Len = ::GetTempPathW(MAX_PATH, Buffer);
		if (Len > 0)
		{
			// 简化：UTF-16 -> UTF-8
			int Needed = ::WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, nullptr, 0, nullptr, nullptr);
			FSKString Out;
			Out.resize(static_cast<size_t>(Needed > 0 ? Needed - 1 : 0));
			if (Needed > 0)
			{
				::WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, Out.data(), Needed, nullptr, nullptr);
			}
			return Out;
		}
		return {};
	#else
		return "/tmp/";
	#endif
	}
}
