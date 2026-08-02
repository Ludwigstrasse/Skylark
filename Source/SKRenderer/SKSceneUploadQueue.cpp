#include "SKRenderer/SKSceneUploadQueue.h"

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKRenderer/SKGpuResidentBufferCache.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    FSKSceneUploadQueue& FSKSceneUploadQueue::Get()
    {
        static FSKSceneUploadQueue GQueue;
        return GQueue;
    }

    void FSKSceneUploadQueue::Reset()
    {
        Requests.clear();
        Stats = {};
    }

    void FSKSceneUploadQueue::QueueVisibleScene(const FSKSceneView& SceneView, uint32 Flags)
    {
        auto QueueGeometry = [&](uint64 GeometryKey)
        {
            if (GeometryKey == 0)
            {
                return;
            }
            auto It = Requests.find(GeometryKey);
            if (It == Requests.end())
            {
                Requests.emplace(GeometryKey, Flags);
                ++Stats.QueuedGeometryCount;
            }
            else
            {
                It->second |= Flags;
            }
        };

        for (const auto& Proxy : SceneView.VisibleProxies)
        {
            QueueGeometry(Proxy.GeometryKey);
        }

        for (const auto& Batch : SceneView.VisibleInstances)
        {
            QueueGeometry(Batch.GeometryKey);
        }
    }

    void FSKSceneUploadQueue::Resolve(ISKRHICommandList& Cmd, const FSKSceneView& SceneView)
    {
        if (!SceneView.GeometryRegistry)
        {
            return;
        }

        auto& Cache = FSKGpuResidentBufferCache::Get();
        ++Stats.ResolveCount;

        for (const auto& KV : Requests)
        {
            const uint64 GeometryKey = KV.first;
            const uint32 Flags = KV.second;
            const FSKGeometryData* Geo = SceneView.GeometryRegistry->Find(GeometryKey);
            if (!Geo)
            {
                continue;
            }

            if ((Flags & SK_SceneUpload_Mesh) != 0u)
            {
                ++Stats.MeshRequestCount;
                const auto& MeshBuffers = Cache.GetOrCreateMeshBuffers(Cmd, *Geo);
                if (MeshBuffers.bResidentSupported)
                {
                    ++Stats.MeshResidentReadyCount;
                }
            }

            if ((Flags & SK_SceneUpload_Edge) != 0u)
            {
                ++Stats.EdgeRequestCount;
                const auto& EdgeBuffers = Cache.GetOrCreateEdgeBuffers(Cmd, *Geo);
                if (EdgeBuffers.bResidentSupported)
                {
                    ++Stats.EdgeResidentReadyCount;
                }
            }
        }
    }

    const FSKSceneUploadStats& FSKSceneUploadQueue::GetStats() const
    {
        return Stats;
    }
}
