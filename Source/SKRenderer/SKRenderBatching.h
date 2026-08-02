#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
    struct FSKBatchedVisibleItem
    {
        const FSKMatrix4f* LocalToWorld = nullptr;
        uint32 HitProxyId = 0;
        int32 SectionIndex = -1;
    };

    struct FSKGeometryDrawBatch
    {
        uint64 GeometryKey = 0;
        int32 SectionIndex = -1;
        TArray<FSKBatchedVisibleItem> Items;
    };

    struct FSKInstancedVisibleItem
    {
        FSKMatrix4f LocalToWorld{};
        uint32 HitProxyId = 0;
        int32 SectionIndex = -1;
    };

    struct FSKInstancedGeometryBatch
    {
        uint64 GeometryKey = 0;
        int32 SectionIndex = -1;
        TArray<FSKInstancedVisibleItem> Instances;

        bool IsInstanceCandidate() const
        {
            return Instances.size() > 1;
        }
    };

    class FSKRenderBatching final
    {
    public:
        static void BuildGeometryBatches(const FSKSceneView& SceneView, TArray<FSKGeometryDrawBatch>& OutBatches);
        static void BuildInstancedGeometryBatches(const FSKSceneView& SceneView, TArray<FSKInstancedGeometryBatch>& OutBatches);
    };
}
