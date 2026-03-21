#pragma once
#include "SKScene/SKSceneGraph.h"

namespace Skylark
{
	/**
	 * Paged LOD (V10)
	 * - OSG-inspired: choose among LOD children based on distance.
	 * - The pager (SKDatabasePager) can be used to load missing LOD payloads asynchronously.
	 */
	class FSKPagedLODNode final : public FSKNode
	{
	public:
		explicit FSKPagedLODNode(FSKString InName) : FSKNode(std::move(InName)) {}

		struct FSKLODChild
		{
			float MinDistance = 0.0f;
			float MaxDistance = 1e9f;
			TSharedPtr<FSKNode> Child;
		};

		void AddLOD(float MinDistance, float MaxDistance, TSharedPtr<FSKNode> Child)
		{
			FSKLODChild L;
			L.MinDistance = MinDistance;
			L.MaxDistance = MaxDistance;
			L.Child = std::move(Child);
			LODs.push_back(std::move(L));
		}

		int32 SelectLOD(float Distance) const
		{
			for (int32 i = 0; i < (int32)LODs.size(); ++i)
			{
				if (Distance >= LODs[i].MinDistance && Distance < LODs[i].MaxDistance)
				{
					return i;
				}
			}
			return (LODs.empty() ? -1 : 0);
		}

		const TArray<FSKLODChild>& GetLODs() const { return LODs; }

	private:
		TArray<FSKLODChild> LODs;
	};
}
