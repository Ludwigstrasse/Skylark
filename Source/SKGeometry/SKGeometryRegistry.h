#pragma once
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
	/**
	 * GeometryRegistry (engine-owned)
	 * - Stores CPU-side geometry and derived data.
	 * - Renderer/SceneGraph reference geometry only by GeometryKey (uint64).
	 *
	 * V6: in-memory only. V7+: add streaming, GPU upload cache, eviction.
	 */
	class FSKGeometryRegistry final
	{
	public:
		uint64 RegisterMesh(FSKString DebugName, const FSKMeshData& Mesh, const FSKGeometryBuildSettings& Settings = {});
		const FSKGeometryData* Find(uint64 GeometryKey) const;

		void Clear();

	private:
		uint64 NextKey = 1;
		TArray<FSKGeometryData> Items;
	};
}
