#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * SKVulkanRHI (V17)
	 * - Extends the V15 device bootstrap into a first presentation-oriented backend scaffold.
	 *
	 * V17 scope:
	 *   1) runtime-loaded Vulkan loader (still no SDK link requirement)
	 *   2) instance / physical device / logical device / graphics queue bootstrap
	 *   3) optional surface + swapchain + present path bootstrap (Win32 path wired; other platforms stay headless)
	 *   4) internal shader/pipeline scaffolds for the future public RHI shader interface
	 *   5) preserved compatibility with Skylark's unified RHI selection / recreation architecture
 *   6) first public triangle-draw command-surface parity with D3D11 / OpenGL (record/scaffold path in this version)
	 *
	 * Important boundary:
	 * - This version establishes the Vulkan presentation architecture and first pipeline scaffolding,
	 *   but the public RHI now exposes a minimal shader / graphics-pipeline surface, and the backend owns
	 *   a first command-pool / command-buffer / queue-submit execution chain. Full swapchain-attached
	 *   render pass and real VkPipeline compilation remain staged for the next iteration.
	 */
	bool SKIsVulkanRHISupported();
	TUniquePtr<ISKRHIDevice> SKCreateVulkanRHIDevice();
}
