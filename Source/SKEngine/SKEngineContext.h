#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKEngine/SKEngine.h"
#include "SKEngine/SKSubsystemCollection.h"
#include "SKRHI/SKRHI.h"
#include "SKRenderer/SKRenderer.h"

namespace Skylark
{
	/**
	 * Engine Context (UE: FEngineLoop + GEngine services, but embedded & instance-based).
	 * - Owns shared services: RHI device, renderer pipeline, viewport list.
	 */
	class FSKEngineContext
	{
	public:
		FSKEngineInitParams InitParams{};

		FSKSubsystemCollection Subsystems;

		TUniquePtr<ISKRHIDevice> RHIDevice;
		TUniquePtr<ISKRenderPipeline> Pipeline;

		bool bLegacyEnabled = false;
	};
}
