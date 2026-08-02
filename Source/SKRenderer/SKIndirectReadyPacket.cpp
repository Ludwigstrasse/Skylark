#include "SKRenderer/SKIndirectReadyPacket.h"

namespace Skylark
{
    FSKIndirectReadyGeometryPacket FSKIndirectReadyPacketBuilder::Build(const FSKGeometryData& Geometry, const FSKMeshDrawPacket& Packet)
    {
        FSKIndirectReadyGeometryPacket Result;
        Result.GeometryKey = Geometry.GeometryKey;
        Result.DebugName = Geometry.DebugName;
        Result.bHasIndices = (Packet.TotalIndexCount > 0);
        Result.bCanBeInstanced = Result.bHasIndices && (Packet.Sections.size() > 0);
        Result.Sections.reserve(Packet.Sections.size());

        for (SIZE_T SectionIndex = 0; SectionIndex < Packet.Sections.size(); ++SectionIndex)
        {
            const auto& Section = Packet.Sections[SectionIndex];
            FSKIndirectReadySectionPacket Entry;
            Entry.SectionIndex = SectionIndex;
            Entry.VertexCount = static_cast<uint32>(Section.Vertices.size());
            Entry.IndexCount = static_cast<uint32>(Section.Indices.size());
            Entry.SectionHitProxyId = Section.SectionHitProxyId;
            Entry.bCanBeInstanced = !Section.Vertices.empty() && !Section.Indices.empty();
            Result.Sections.push_back(std::move(Entry));
        }

        return Result;
    }

    FSKIndirectReadyPacketCache& FSKIndirectReadyPacketCache::Get()
    {
        static FSKIndirectReadyPacketCache GCache;
        return GCache;
    }

    const FSKIndirectReadyGeometryPacket& FSKIndirectReadyPacketCache::GetOrBuild(const FSKGeometryData& Geometry, const FSKMeshDrawPacket& Packet)
    {
        auto It = Packets.find(Geometry.GeometryKey);
        if (It != Packets.end())
        {
            return It->second;
        }

        auto Result = Packets.emplace(Geometry.GeometryKey, FSKIndirectReadyPacketBuilder::Build(Geometry, Packet));
        return Result.first->second;
    }

    void FSKIndirectReadyPacketCache::Clear()
    {
        Packets.clear();
    }
}
