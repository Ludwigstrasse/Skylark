#pragma once
#include "SKPlatform/SKPlatform.h"

namespace Skylark
{
	class FSKLinuxPlatformMisc final : public ISKPlatformMisc
	{
	public:
		ESKPlatform GetPlatform() const override { return ESKPlatform::Linux; }
		uint32 GetCoreCount() const override;
		FSKString GetUserTempDir() const override;
	};
}
