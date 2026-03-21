#pragma once

#include "SKShaderCore/SKShaderCore.h"

namespace Skylark
{
	/**
	 * ShaderCompiler (V13)
	 * - UE5-aligned: central compile entry point; backend-specific code lives in .cpp.
	 * - Default implementation compiles HLSL on Windows via D3DCompiler.
	 * - On non-Windows platforms, returns a clear error (Skylark can still build with NullRHI).
	 */

	TUniquePtr<ISKShaderCompiler> SKCreateDefaultShaderCompiler();
}
