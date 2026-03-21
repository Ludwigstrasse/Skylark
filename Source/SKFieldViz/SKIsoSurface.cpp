#include "SKFieldViz/SKIsoSurface.h"

namespace Skylark
{
	bool FSKIsoSurface::BuildIsoSurface(const FSKIsoSurfaceRequest& Req, const FSKGeometryData& Geo, const FSKFieldSet& Fields, FSKMeshData& OutMesh)
	{
		(void)Req;
		(void)Geo;
		(void)Fields;
		OutMesh.Sections.clear();
		return false; // V10 skeleton
	}
}
