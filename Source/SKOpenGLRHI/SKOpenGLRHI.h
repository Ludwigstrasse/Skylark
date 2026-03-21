#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * SKOpenGLRHI (V14)
	 * - First OpenGL backend for Skylark.
	 * - Current scope: device + WGL swap chain + clear + line overlay + CPU-backed offscreen textures.
	 * - The design purpose is to land a usable OpenGL backend while keeping the RHI surface aligned with
	 *   D3D/Vulkan-style backend switching.
	 */
	TUniquePtr<ISKRHIDevice> SKCreateOpenGLRHIDevice();
}
