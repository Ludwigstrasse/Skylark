#pragma once
#include "SKEngine/SKEngine.h"
#include "SKScene/SKSceneGraph.h"
#include "SKGeometry/SKGeometryRegistry.h"

namespace Skylark
{
	/**
	 * Optional engine extension interface for host integration.
	 *
	 * SolidDesigner should treat Skylark as an embedded library:
	 * - Host owns UI and event loop.
	 * - Engine owns scene/geometry registries (renderer-agnostic data).
	 *
	 * This interface avoids polluting the minimal ISKEngine surface, while still
	 * allowing advanced integration by dynamic_cast.
	 */
	class ISKEngineSceneAccess
	{
	public:
		virtual ~ISKEngineSceneAccess() = default;

		virtual TSharedPtr<FSKSceneGraph> GetSceneGraph() const = 0;
		virtual FSKGeometryRegistry* GetGeometryRegistry() const = 0;
	};
}
