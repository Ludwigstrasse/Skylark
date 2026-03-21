#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"

#include <algorithm>

namespace Skylark
{
	/**
	 * Geometry data structures (engine-side)
	 *
	 * Design notes (UE5/OSG/OCCT aligned):
	 * - UE: Renderer consumes RenderProxies which reference geometry resources by key/handle.
	 * - OSG: Geometry is a drawable with vertex arrays and primitive sets, referenced by nodes.
	 * - OCCT: CAD kernel provides BREP; engine stores tessellation for display and selection.
	 *
	 * V6: Geometry is CPU-side mesh with optional derived wireframe (edge indices).
	 *     GPU upload & draw submission are V7+ (via SKRHI resource model expansion).
	 */

	struct FSKVertex
	{
		FSKVector3f Position{};
		FSKVector3f Normal{};
		float U = 0.0f;
		float V = 0.0f;
	};

	struct FSKMeshSection
	{
		// Stable sub-entity id (Face/Body/Element/Cell...). Used by host logic.
		FSKObjectId OwnerId{};

		// GPU selection id (HitProxyTable index). Used by SelectionIdBuffer.
		uint32 HitProxyId = 0;

		TArray<FSKVertex> Vertices;
		TArray<uint32> Indices; // triangle list
	};

	struct FSKMeshData
	{
		TArray<FSKMeshSection> Sections;
	};

	struct FSKWireframeData
	{
		// line list indices (pairs of vertex indices)
		TArray<uint32> LineIndices;
	};

	struct FSKEdgeIndexLists
	{
		// Line indices (pairs of vertex indices), by category
		TArray<uint32> All;
		TArray<uint32> Boundary;
		TArray<uint32> Sharp;
	};

	struct FSKMeshDerivedData
	{
		// One wireframe per section (legacy alias: all unique triangle edges).
		TArray<FSKWireframeData> SectionWireframes;

		// Categorized edges per section (V7): All/Boundary/Sharp.
		TArray<FSKEdgeIndexLists> SectionEdges;
	};

	struct FSKGeometryBuildSettings
	{
		bool bBuildWireframe = true;
		bool bBuildEdgeCategories = true;
		float CreaseAngleDeg = 30.0f;
		bool bWeldVertices = false; // future: weld for better wireframe/crease detection
	};

	struct FSKAabb
	{
		FSKVector3f Min{ +INFINITY, +INFINITY, +INFINITY };
		FSKVector3f Max{ -INFINITY, -INFINITY, -INFINITY };

		void Encapsulate(const FSKVector3f& P)
		{
			Min.X = std::min(Min.X, P.X);
			Min.Y = std::min(Min.Y, P.Y);
			Min.Z = std::min(Min.Z, P.Z);
			Max.X = std::max(Max.X, P.X);
			Max.Y = std::max(Max.Y, P.Y);
			Max.Z = std::max(Max.Z, P.Z);
		}

		bool IsValid() const { return Min.X <= Max.X && Min.Y <= Max.Y && Min.Z <= Max.Z; }
	};

	struct FSKGeometryData
	{
		uint64 GeometryKey = 0;
		FSKString DebugName;

		FSKMeshData Mesh;
		FSKMeshDerivedData Derived;

		FSKAabb Bounds;
	};
}
