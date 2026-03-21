#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKGeometry/SKGeometryTypes.h"
#include "SKGeometry/SKGeometryRegistry.h"
#include "SKField/SKField.h"

namespace Skylark
{
	struct FSKIsoSurfaceRequest
	{
		uint64 GeometryKey = 0;
		FSKFieldKey Field;
		float IsoValue = 0.0f;
	};

	class FSKIsoSurface
	{
	public:
		static bool BuildIsoSurface(const FSKIsoSurfaceRequest& Req, const FSKGeometryData& Geo, const FSKFieldSet& Fields, FSKMeshData& OutMesh);
	};
}
