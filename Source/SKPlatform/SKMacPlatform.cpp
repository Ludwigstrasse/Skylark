#include "SKPlatform/SKMacPlatform.h"

#if SK_PLATFORM_MAC
	#include <cstdlib>
	#include <thread>
#endif

namespace Skylark
{
	uint32 FSKMacPlatformMisc::GetCoreCount() const
	{
#if SK_PLATFORM_MAC
		const unsigned int HC = std::thread::hardware_concurrency();
		return static_cast<uint32>(HC > 0 ? HC : 1);
#else
		return 1;
#endif
	}

	FSKString FSKMacPlatformMisc::GetUserTempDir() const
	{
#if SK_PLATFORM_MAC
		const char* Tmp = std::getenv("TMPDIR");
		if (!Tmp || !Tmp[0]) Tmp = "/tmp";
		return FSKString(Tmp);
#else
		return {};
#endif
	}
}
