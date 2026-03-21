#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * SKD3D12RHI (V10)
	 * - UE5-aligned backend implementation for ISKRHIDevice using Direct3D 12.
	 * - Optional and Windows-only (SK_WITH_D3D12_RHI).
	 *
	 * V10 scope:
	 * - Device/Queue/Fence
	 * - SwapChain (flip-discard)
	 * - Clear + Present
	 * - Markers/timestamps are left as stubs (V11+)
	 */
	TUniquePtr<ISKRHIDevice> SKCreateD3D12RHIDevice();
}
