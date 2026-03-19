#pragma once
#include "SKEngine/SKEngine.h"

namespace Skylark
{
	/**
	 * Legacy bridge wrapper (SKEngine2/VSEngine2).
	 * - Encapsulates legacy global singletons behind an instance.
	 * - Heavy legacy headers are isolated in the .cpp.
	 */
	class FSKLegacyEngine2Context
	{
	public:
		FSKLegacyEngine2Context();
		~FSKLegacyEngine2Context();

		bool Init(const FSKEngineInitParams& Params);
		void Shutdown();
		void Tick(float DeltaSeconds);

		bool IsActive() const { return bActive; }

	private:
		bool bActive = false;
		void* Impl = nullptr;
	};
}
