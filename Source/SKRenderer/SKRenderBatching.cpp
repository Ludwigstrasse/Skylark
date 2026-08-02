#include "SKRenderer/SKRenderBatching.h"

namespace Skylark
{
    namespace
    {
        static uint64 MakeGeometrySectionBatchKey(uint64 GeometryKey, int32 SectionIndex)
        {
            uint64 H = GeometryKey * 0x9E3779B97F4A7C15ull;
            const uint64 SectionKey = static_cast<uint64>(static_cast<int64>(SectionIndex) + 1ll);
            H ^= (SectionKey + 0xBF58476D1CE4E5B9ull) + (H << 6) + (H >> 2);
            return H;
        }

        template<typename TBatch>
        static TBatch& GetOrAddBatch(TArray<TBatch>& OutBatches, TMap<uint64, SIZE_T>& BatchIndexByGeometrySection, uint64 GeometryKey, int32 SectionIndex)
        {
            const uint64 BatchKey = MakeGeometrySectionBatchKey(GeometryKey, SectionIndex);
            auto It = BatchIndexByGeometrySection.find(BatchKey);
            if (It != BatchIndexByGeometrySection.end())
            {
                return OutBatches[It->second];
            }

            const SIZE_T NewIndex = OutBatches.size();
            OutBatches.push_back(TBatch{});
            OutBatches.back().GeometryKey = GeometryKey;
            OutBatches.back().SectionIndex = SectionIndex;
            BatchIndexByGeometrySection.emplace(BatchKey, NewIndex);
            return OutBatches.back();
        }
    }

    void FSKRenderBatching::BuildGeometryBatches(const FSKSceneView& SceneView, TArray<FSKGeometryDrawBatch>& OutBatches)
    {
        OutBatches.clear();
        TMap<uint64, SIZE_T> BatchIndexByGeometrySection;

        for (const auto& Proxy : SceneView.VisibleProxies)
        {
            if (Proxy.GeometryKey == 0)
            {
                continue;
            }
            auto& Batch = GetOrAddBatch(OutBatches, BatchIndexByGeometrySection, Proxy.GeometryKey, Proxy.SectionIndex);
            Batch.Items.push_back(FSKBatchedVisibleItem{ &Proxy.LocalToWorld, Proxy.HitProxyId, Proxy.SectionIndex });
        }

        for (const auto& InstanceBatch : SceneView.VisibleInstances)
        {
            if (InstanceBatch.GeometryKey == 0)
            {
                continue;
            }
            auto& Batch = GetOrAddBatch(OutBatches, BatchIndexByGeometrySection, InstanceBatch.GeometryKey, InstanceBatch.SectionIndex);
            for (SIZE_T InstanceIndex = 0; InstanceIndex < InstanceBatch.InstanceLocalToWorld.size(); ++InstanceIndex)
            {
                const uint32 HitProxyId = (InstanceIndex < InstanceBatch.InstanceHitProxyId.size()) ? InstanceBatch.InstanceHitProxyId[InstanceIndex] : 0u;
                Batch.Items.push_back(FSKBatchedVisibleItem{ &InstanceBatch.InstanceLocalToWorld[InstanceIndex], HitProxyId, InstanceBatch.SectionIndex });
            }
        }
    }

    void FSKRenderBatching::BuildInstancedGeometryBatches(const FSKSceneView& SceneView, TArray<FSKInstancedGeometryBatch>& OutBatches)
    {
        OutBatches.clear();
        TMap<uint64, SIZE_T> BatchIndexByGeometrySection;

        for (const auto& Proxy : SceneView.VisibleProxies)
        {
            if (Proxy.GeometryKey == 0)
            {
                continue;
            }

            auto& Batch = GetOrAddBatch(OutBatches, BatchIndexByGeometrySection, Proxy.GeometryKey, Proxy.SectionIndex);
            FSKInstancedVisibleItem Item;
            Item.LocalToWorld = Proxy.LocalToWorld;
            Item.HitProxyId = Proxy.HitProxyId;
            Item.SectionIndex = Proxy.SectionIndex;
            Batch.Instances.push_back(std::move(Item));
        }

        for (const auto& InstanceBatch : SceneView.VisibleInstances)
        {
            if (InstanceBatch.GeometryKey == 0)
            {
                continue;
            }

            auto& Batch = GetOrAddBatch(OutBatches, BatchIndexByGeometrySection, InstanceBatch.GeometryKey, InstanceBatch.SectionIndex);
            for (SIZE_T InstanceIndex = 0; InstanceIndex < InstanceBatch.InstanceLocalToWorld.size(); ++InstanceIndex)
            {
                FSKInstancedVisibleItem Item;
                Item.LocalToWorld = InstanceBatch.InstanceLocalToWorld[InstanceIndex];
                Item.HitProxyId = (InstanceIndex < InstanceBatch.InstanceHitProxyId.size()) ? InstanceBatch.InstanceHitProxyId[InstanceIndex] : 0u;
                Item.SectionIndex = InstanceBatch.SectionIndex;
                Batch.Instances.push_back(std::move(Item));
            }
        }
    }
}
