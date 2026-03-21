#include "SKPlatform/SKLinuxPlatform.h"

#if SK_PLATFORM_LINUX
	#include <cstdlib>
	#include <thread>
#endif

namespace Skylark
{
	uint32 FSKLinuxPlatformMisc::GetCoreCount() const
	{
#if SK_PLATFORM_LINUX
		const unsigned int HC = std::thread::hardware_concurrency();
		return static_cast<uint32>(HC > 0 ? HC : 1);
#else
		return 1;
#endif
	}

	FSKString FSKLinuxPlatformMisc::GetUserTempDir() const
	{
#if SK_PLATFORM_LINUX
		const char* Tmp = std::getenv("TMPDIR");
		if (!Tmp || !Tmp[0]) Tmp = "/tmp";
		return FSKString(Tmp);
#else
		return {};
#endif
	}
}
