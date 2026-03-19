#include "SKPlatform/SKPlatform.h"
#include "SKPlatform/SKWindowsPlatform.h"

namespace Skylark
{
	static FSKWindowsPlatformMisc GWindowsMisc;

	ISKPlatformMisc& SKGetPlatformMisc()
	{
		// TODO: Linux/macOS
		return GWindowsMisc;
	}
}
