#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCAD/SKCadTypes.h"

namespace Skylark
{
	/**
	 * HitProxy table (UE: HitProxy / OSG: picking masks / OCCT: SelectMgr owner mapping)
	 *
	 * - Assigns small integer ids to TopoId for GPU id-buffer picking.
	 * - Encoding uses RGBA8 (24-bit usable; alpha reserved).
	 *
	 * NOTE: For commercial CAD, ids must be stable across frames for hover/drag workflows.
	 *       V5 provides the infrastructure; policies are pluggable (hash map, LRU, per-document stable tables).
	 */

	struct FSKHitProxyId
	{
		uint32 Value = 0;
		static FSKHitProxyId Invalid() { return FSKHitProxyId{0}; }
		bool IsValid() const { return Value != 0; }
	};

	inline void SKEncodeHitProxyIdRGBA8(uint32 Id, uint8 OutRGBA[4])
	{
		OutRGBA[0] = (uint8)((Id >> 0) & 0xFF);
		OutRGBA[1] = (uint8)((Id >> 8) & 0xFF);
		OutRGBA[2] = (uint8)((Id >> 16) & 0xFF);
		OutRGBA[3] = (uint8)((Id >> 24) & 0xFF);
	}

	inline uint32 SKDecodeHitProxyIdRGBA8(const uint8 RGBA[4])
	{
		return (uint32)RGBA[0] | ((uint32)RGBA[1] << 8) | ((uint32)RGBA[2] << 16) | ((uint32)RGBA[3] << 24);
	}

	class FSKHitProxyTable
	{
	public:
		void ResetFrame()
		{
			// V5 default: per-frame rebuild (safe + simple)
			Entries.clear();
			NextId = 1;
		}

		FSKHitProxyId Register(const FSKTopoId& Topo)
		{
			if (!Topo.IsValid())
			{
				return FSKHitProxyId::Invalid();
			}

			Entries.push_back(Topo);
			const uint32 Id = NextId++;
			return FSKHitProxyId{Id};
		}

		bool Resolve(const FSKHitProxyId& Id, FSKTopoId& OutTopo) const
		{
			if (!Id.IsValid())
			{
				return false;
			}
			const uint32 Index = Id.Value - 1;
			if (Index >= (uint32)Entries.size())
			{
				return false;
			}
			OutTopo = Entries[Index];
			return true;
		}

	private:
		TArray<FSKTopoId> Entries;
		uint32 NextId = 1;
	};
}
