#pragma once
#include "SKPlatform/SKPlatform.h"

namespace Skylark
{
	class FSKWindowsPlatformMisc final : public ISKPlatformMisc
	{
	public:
		ESKPlatform GetPlatform() const override { return ESKPlatform::Windows; }
		uint32 GetCoreCount() const override;
		FSKString GetUserTempDir() const override;
	};
}
