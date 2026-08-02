#include "SKRenderer/SKMeshDrawPacket.h"

#include <algorithm>

namespace Skylark
{
    namespace
    {
        static FSKVector3f MakeStableBaseColor(uint64 GeometryKey, uint32 SectionIndex)
        {
            uint64 Hash = GeometryKey * 11400714819323198485ull;
            Hash ^= (uint64)SectionIndex * 0x9E3779B97F4A7C15ull;
            const float R = 0.55f + 0.30f * (float)((Hash >> 0) & 0xFFu) / 255.0f;
            const float G = 0.58f + 0.25f * (float)((Hash >> 8) & 0xFFu) / 255.0f;
            const float B = 0.62f + 0.20f * (float)((Hash >> 16) & 0xFFu) / 255.0f;
            return FSKVector3f(std::min(R, 0.92f), std::min(G, 0.90f), std::min(B, 0.88f));
        }
    }

    FSKMeshDrawPacket FSKMeshDrawPacketBuilder::Build(const FSKGeometryData& Geometry)
    {
        FSKMeshDrawPacket Packet;
        Packet.GeometryKey = Geometry.GeometryKey;
        Packet.DebugName = Geometry.DebugName;
        Packet.Sections.reserve(Geometry.Mesh.Sections.size());

        for (SIZE_T SectionIndex = 0; SectionIndex < Geometry.Mesh.Sections.size(); ++SectionIndex)
        {
            const auto& Section = Geometry.Mesh.Sections[SectionIndex];
            if (Section.Vertices.empty() || Section.Indices.empty() || (Section.Indices.size() % 3) != 0)
            {
                continue;
            }

            FSKMeshSectionDrawPacket PacketSection;
            PacketSection.OwnerId = Section.OwnerId;
            PacketSection.SectionHitProxyId = Section.HitProxyId;
            PacketSection.BaseColor = MakeStableBaseColor(Geometry.GeometryKey, static_cast<uint32>(SectionIndex));
            PacketSection.Vertices.reserve(Section.Vertices.size());
            PacketSection.Indices.reserve(Section.Indices.size());

            for (const auto& Vertex : Section.Vertices)
            {
                FSKMeshPacketVertex PacketVertex;
                PacketVertex.PositionLocal = Vertex.Position;
                PacketVertex.NormalLocal = Vertex.Normal;
                PacketSection.Vertices.push_back(PacketVertex);
            }

            for (const uint32 Index : Section.Indices)
            {
                if (Index < PacketSection.Vertices.size())
                {
                    PacketSection.Indices.push_back(Index);
                }
            }

            if (!PacketSection.Vertices.empty() && !PacketSection.Indices.empty())
            {
                Packet.TotalVertexCount += PacketSection.Vertices.size();
                Packet.TotalIndexCount += PacketSection.Indices.size();
                Packet.Sections.push_back(std::move(PacketSection));
            }
        }

        return Packet;
    }

    const FSKMeshDrawPacket& FSKMeshDrawPacketCache::GetOrBuild(const FSKGeometryData& Geometry)
    {
        auto It = Packets.find(Geometry.GeometryKey);
        if (It != Packets.end())
        {
            return It->second;
        }

        FSKMeshDrawPacket Packet = FSKMeshDrawPacketBuilder::Build(Geometry);
        auto Result = Packets.emplace(Geometry.GeometryKey, std::move(Packet));
        return Result.first->second;
    }

    void FSKMeshDrawPacketCache::Clear()
    {
        Packets.clear();
    }
}
