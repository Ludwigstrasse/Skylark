#include "SKPlatform/SKPlatform.h"

#if SK_PLATFORM_WINDOWS
	#include "SKPlatform/SKWindowsPlatform.h"
#elif SK_PLATFORM_LINUX
	#include "SKPlatform/SKLinuxPlatform.h"
#elif SK_PLATFORM_MAC
	#include "SKPlatform/SKMacPlatform.h"
#endif

namespace Skylark
{
#if SK_PLATFORM_WINDOWS
	static FSKWindowsPlatformMisc GPlatformMisc;
#elif SK_PLATFORM_LINUX
	static FSKLinuxPlatformMisc GPlatformMisc;
#elif SK_PLATFORM_MAC
	static FSKMacPlatformMisc GPlatformMisc;
#else
	class FSKUnknownPlatformMisc final : public ISKPlatformMisc
	{
	public:
		ESKPlatform GetPlatform() const override { return ESKPlatform::Unknown; }
		uint32 GetCoreCount() const override { return 1; }
		FSKString GetUserTempDir() const override { return {}; }
	};
	static FSKUnknownPlatformMisc GPlatformMisc;
#endif

	ISKPlatformMisc& SKGetPlatformMisc()
	{
		return GPlatformMisc;
	}
}
