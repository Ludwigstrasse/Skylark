#pragma once
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
	/**
	 * MeshBuilder utilities (CPU-side)
	 * - Build wireframe indices from triangle index buffer (unique edges).
	 * - Compute bounds.
	 *
	 * UE reference:
	 * - FStaticMeshLODResources / BuildAdjacencyBuffer (conceptual)
	 * OSG reference:
	 * - osgUtil::SmoothingVisitor / PrimitiveSet processing (conceptual)
	 */
	class FSKMeshBuilder
	{
	public:
		static FSKAabb ComputeBounds(const FSKMeshSection& Section);
		static FSKAabb ComputeBounds(const FSKMeshData& Mesh);

		// Build unique-edge wireframe for one section.
		static FSKWireframeData BuildWireframe(const FSKMeshSection& Section);

		// Build derived data for full mesh.
		static FSKMeshDerivedData BuildDerived(const FSKMeshData& Mesh, const FSKGeometryBuildSettings& Settings);
	};
}
