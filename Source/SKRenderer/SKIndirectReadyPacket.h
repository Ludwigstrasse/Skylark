#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRenderer/SKMeshDrawPacket.h"
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
    struct FSKIndirectReadySectionPacket
    {
        SIZE_T SectionIndex = 0;
        uint32 VertexCount = 0;
        uint32 IndexCount = 0;
        uint32 SectionHitProxyId = 0;
        bool bCanBeInstanced = false;
    };

    struct FSKIndirectReadyGeometryPacket
    {
        uint64 GeometryKey = 0;
        FSKString DebugName;
        bool bHasIndices = false;
        bool bCanBeInstanced = false;
        TArray<FSKIndirectReadySectionPacket> Sections;
    };

    class FSKIndirectReadyPacketBuilder final
    {
    public:
        static FSKIndirectReadyGeometryPacket Build(const FSKGeometryData& Geometry, const FSKMeshDrawPacket& Packet);
    };

    class FSKIndirectReadyPacketCache final
    {
    public:
        static FSKIndirectReadyPacketCache& Get();

        const FSKIndirectReadyGeometryPacket& GetOrBuild(const FSKGeometryData& Geometry, const FSKMeshDrawPacket& Packet);
        void Clear();

    private:
        TMap<uint64, FSKIndirectReadyGeometryPacket> Packets;
    };
}
