#include "SKGeometry/SKGeometryRegistry.h"
#include "SKGeometry/SKMeshBuilder.h"

namespace Skylark
{
	uint64 FSKGeometryRegistry::RegisterMesh(FSKString DebugName, const FSKMeshData& Mesh, const FSKGeometryBuildSettings& Settings)
	{
		FSKGeometryData G;
		G.GeometryKey = NextKey++;
		G.DebugName = std::move(DebugName);
		G.Mesh = Mesh;
		G.Derived = FSKMeshBuilder::BuildDerived(G.Mesh, Settings);
		G.Bounds = FSKMeshBuilder::ComputeBounds(G.Mesh);

		Items.push_back(std::move(G));
		return Items.back().GeometryKey;
	}

	const FSKGeometryData* FSKGeometryRegistry::Find(uint64 GeometryKey) const
	{
		for (const auto& It : Items)
		{
			if (It.GeometryKey == GeometryKey)
			{
				return &It;
			}
		}
		return nullptr;
	}

	void FSKGeometryRegistry::Clear()
	{
		Items.clear();
		NextKey = 1;
	}
}
