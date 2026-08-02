#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
    struct FSKMeshPacketVertex
    {
        FSKVector3f PositionLocal{};
        FSKVector3f NormalLocal{};
    };

    struct FSKMeshSectionDrawPacket
    {
        FSKObjectId OwnerId{};
        uint32 SectionHitProxyId = 0;
        FSKVector3f BaseColor = FSKVector3f(0.75f, 0.77f, 0.80f);
        TArray<FSKMeshPacketVertex> Vertices;
        TArray<uint32> Indices;
    };

    struct FSKMeshDrawPacket
    {
        uint64 GeometryKey = 0;
        FSKString DebugName;
        TArray<FSKMeshSectionDrawPacket> Sections;
        SIZE_T TotalVertexCount = 0;
        SIZE_T TotalIndexCount = 0;
    };

    class FSKMeshDrawPacketBuilder final
    {
    public:
        static FSKMeshDrawPacket Build(const FSKGeometryData& Geometry);
    };

    class FSKMeshDrawPacketCache final
    {
    public:
        const FSKMeshDrawPacket& GetOrBuild(const FSKGeometryData& Geometry);
        void Clear();

    private:
        TMap<uint64, FSKMeshDrawPacket> Packets;
    };
}
