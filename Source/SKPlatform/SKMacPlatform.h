#pragma once
#include "SKPlatform/SKPlatform.h"

namespace Skylark
{
	class FSKMacPlatformMisc final : public ISKPlatformMisc
	{
	public:
		ESKPlatform GetPlatform() const override { return ESKPlatform::Mac; }
		uint32 GetCoreCount() const override;
		FSKString GetUserTempDir() const override;
	};
}
