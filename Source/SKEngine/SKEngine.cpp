#include "SKEngine/SKEngine.h"
#include "SKRuntime/SKRuntimeEngine.h"

namespace Skylark
{
	ISKEngine* SKCreateEngine()
	{
		return new FSKRuntimeEngine();
	}

	void SKDestroyEngine(ISKEngine* Engine)
	{
		delete Engine;
	}
}
