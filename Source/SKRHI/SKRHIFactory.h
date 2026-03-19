#pragma once
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	// Factory helpers (UE: DynamicRHI)
	TUniquePtr<ISKRHIDevice> SKCreateRHIDevice(const FSKRHIDeviceDesc& Desc);
}
