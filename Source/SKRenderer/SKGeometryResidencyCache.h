#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKGeometry/SKGeometryTypes.h"
#include "SKRenderer/SKMeshDrawPacket.h"
#include "SKRenderer/SKEdgeDrawPacket.h"

namespace Skylark
{
    struct FSKGeometryResidencyStats
    {
        SIZE_T MeshPacketHits = 0;
        SIZE_T MeshPacketMisses = 0;
        SIZE_T EdgePacketHits = 0;
        SIZE_T EdgePacketMisses = 0;
        SIZE_T ResidentMeshGeometryCount = 0;
        SIZE_T ResidentEdgeGeometryCount = 0;
        SIZE_T ResidentMeshVertexCount = 0;
        SIZE_T ResidentMeshIndexCount = 0;
        SIZE_T ResidentEdgeLineCount = 0;
    };

    class FSKGeometryResidencyCache final
    {
    public:
        static FSKGeometryResidencyCache& Get();

        const FSKMeshDrawPacket& GetOrBuildMeshPacket(const FSKGeometryData& Geometry);
        const FSKEdgeDrawPacket& GetOrBuildEdgePacket(const FSKGeometryData& Geometry);

        void Clear();
        const FSKGeometryResidencyStats& GetStats() const;

    private:
        void RebuildStats();

    private:
        TMap<uint64, FSKMeshDrawPacket> MeshPackets;
        TMap<uint64, FSKEdgeDrawPacket> EdgePackets;
        FSKGeometryResidencyStats Stats{};
    };
}
