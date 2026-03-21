#pragma once
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
	/**
	 * CAD mesh compatibility header.
	 *
	 * V6: the engine-wide geometry types live in SKGeometry.
	 * This header keeps historical include paths stable for SKCAD users.
	 */
	using FSKVertex = Skylark::FSKVertex;
	using FSKMeshSection = Skylark::FSKMeshSection;
	using FSKMeshData = Skylark::FSKMeshData;
}
