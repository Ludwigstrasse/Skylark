#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRenderer/SKRenderBatching.h"

namespace Skylark
{
    struct FSKInstanceDrawItem
    {
        FSKMatrix4f LocalToWorld{};
        uint32 HitProxyId = 0;
    };

    struct FSKInstanceDrawRun
    {
        uint64 GeometryKey = 0;
        TArray<FSKInstanceDrawItem> Items;
        bool bInstanceCandidate = false;
        bool bIndirectReady = false;
    };

    class FSKInstanceDrawPacketBuilder final
    {
    public:
        static void BuildRuns(const TArray<FSKInstancedGeometryBatch>& Batches, TArray<FSKInstanceDrawRun>& OutRuns);
    };
}
