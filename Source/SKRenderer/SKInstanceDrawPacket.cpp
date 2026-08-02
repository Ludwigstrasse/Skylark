#include "SKRenderer/SKInstanceDrawPacket.h"

namespace Skylark
{
    void FSKInstanceDrawPacketBuilder::BuildRuns(const TArray<FSKInstancedGeometryBatch>& Batches, TArray<FSKInstanceDrawRun>& OutRuns)
    {
        OutRuns.clear();
        OutRuns.reserve(Batches.size());

        for (const auto& Batch : Batches)
        {
            if (Batch.GeometryKey == 0 || Batch.Instances.empty())
            {
                continue;
            }

            FSKInstanceDrawRun Run;
            Run.GeometryKey = Batch.GeometryKey;
            Run.bInstanceCandidate = (Batch.Instances.size() > 1);
            Run.bIndirectReady = Run.bInstanceCandidate;

            Run.Items.reserve(Batch.Instances.size());
            for (const auto& Instance : Batch.Instances)
            {
                FSKInstanceDrawItem Item;
                Item.LocalToWorld = Instance.LocalToWorld;
                Item.HitProxyId = Instance.HitProxyId;
                Run.Items.push_back(std::move(Item));
            }

            OutRuns.push_back(std::move(Run));
        }
    }
}
