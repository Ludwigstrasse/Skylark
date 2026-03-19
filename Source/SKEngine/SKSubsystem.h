#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	class FSKEngineContext;

	/**
	 * UE5-aligned Engine Subsystem interface.
	 * - No globals: subsystems live inside FSKEngineContext.
	 * - Lifetime: Initialize -> Tick (optional) -> Deinitialize
	 */
	class ISKEngineSubsystem
	{
	public:
		virtual ~ISKEngineSubsystem() = default;
		virtual void Initialize(FSKEngineContext& Context) = 0;
		virtual void Deinitialize() = 0;
		virtual void Tick(float DeltaSeconds) { (void)DeltaSeconds; }
	};
}
