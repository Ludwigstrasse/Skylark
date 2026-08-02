#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKGeometry/SKGeometryTypes.h"
#include "SKRenderer/SKRenderer.h"

namespace Skylark
{
    struct FSKEdgePacketLine
    {
        FSKVector3f P0Local{};
        FSKVector3f P1Local{};
    };

    struct FSKEdgeSectionDrawPacket
    {
        FSKObjectId OwnerId{};
        uint32 SectionHitProxyId = 0;
        TArray<FSKEdgePacketLine> All;
        TArray<FSKEdgePacketLine> Boundary;
        TArray<FSKEdgePacketLine> Sharp;
    };

    struct FSKEdgeDrawPacket
    {
        uint64 GeometryKey = 0;
        FSKString DebugName;
        TArray<FSKEdgeSectionDrawPacket> Sections;
        SIZE_T TotalAllLineCount = 0;
        SIZE_T TotalBoundaryLineCount = 0;
        SIZE_T TotalSharpLineCount = 0;
    };

    class FSKEdgeDrawPacketBuilder final
    {
    public:
        static FSKEdgeDrawPacket Build(const FSKGeometryData& Geometry);
        static const TArray<FSKEdgePacketLine>& GetLines(const FSKEdgeSectionDrawPacket& Section, FSKEdgeRenderSettings::EDisplay Display);
    };

    class FSKEdgeDrawPacketCache final
    {
    public:
        const FSKEdgeDrawPacket& GetOrBuild(const FSKGeometryData& Geometry);
        const FSKEdgeDrawPacket* Find(uint64 GeometryKey) const;
        void Clear();

    private:
        TMap<uint64, FSKEdgeDrawPacket> Packets;
    };
}
