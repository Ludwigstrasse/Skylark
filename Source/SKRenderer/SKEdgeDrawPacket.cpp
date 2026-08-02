#include "SKRenderer/SKEdgeDrawPacket.h"

namespace Skylark
{
    namespace
    {
        static void AppendEdgeLines(const FSKMeshSection& Section, const TArray<uint32>& LineIndices, TArray<FSKEdgePacketLine>& OutLines)
        {
            if (Section.Vertices.empty() || LineIndices.size() < 2)
            {
                return;
            }

            OutLines.reserve(OutLines.size() + (LineIndices.size() / 2));
            for (SIZE_T Index = 0; Index + 1 < LineIndices.size(); Index += 2)
            {
                const uint32 I0 = LineIndices[Index + 0];
                const uint32 I1 = LineIndices[Index + 1];
                if (I0 >= Section.Vertices.size() || I1 >= Section.Vertices.size())
                {
                    continue;
                }

                FSKEdgePacketLine Line;
                Line.P0Local = Section.Vertices[I0].Position;
                Line.P1Local = Section.Vertices[I1].Position;
                OutLines.push_back(Line);
            }
        }
    }

    FSKEdgeDrawPacket FSKEdgeDrawPacketBuilder::Build(const FSKGeometryData& Geometry)
    {
        FSKEdgeDrawPacket Packet;
        Packet.GeometryKey = Geometry.GeometryKey;
        Packet.DebugName = Geometry.DebugName;
        Packet.Sections.reserve(Geometry.Mesh.Sections.size());

        for (SIZE_T SectionIndex = 0; SectionIndex < Geometry.Mesh.Sections.size(); ++SectionIndex)
        {
            const auto& Section = Geometry.Mesh.Sections[SectionIndex];
            if (Section.Vertices.empty())
            {
                continue;
            }

            FSKEdgeSectionDrawPacket PacketSection;
            PacketSection.OwnerId = Section.OwnerId;
            PacketSection.SectionHitProxyId = Section.HitProxyId;

            if (SectionIndex < Geometry.Derived.SectionEdges.size())
            {
                const auto& SectionEdges = Geometry.Derived.SectionEdges[SectionIndex];
                AppendEdgeLines(Section, SectionEdges.All, PacketSection.All);
                AppendEdgeLines(Section, SectionEdges.Boundary, PacketSection.Boundary);
                AppendEdgeLines(Section, SectionEdges.Sharp, PacketSection.Sharp);
            }
            else if (SectionIndex < Geometry.Derived.SectionWireframes.size())
            {
                AppendEdgeLines(Section, Geometry.Derived.SectionWireframes[SectionIndex].LineIndices, PacketSection.All);
            }

            if (!PacketSection.All.empty() || !PacketSection.Boundary.empty() || !PacketSection.Sharp.empty())
            {
                Packet.TotalAllLineCount += PacketSection.All.size();
                Packet.TotalBoundaryLineCount += PacketSection.Boundary.size();
                Packet.TotalSharpLineCount += PacketSection.Sharp.size();
                Packet.Sections.push_back(std::move(PacketSection));
            }
        }

        return Packet;
    }

    const TArray<FSKEdgePacketLine>& FSKEdgeDrawPacketBuilder::GetLines(const FSKEdgeSectionDrawPacket& Section, FSKEdgeRenderSettings::EDisplay Display)
    {
        switch (Display)
        {
        case FSKEdgeRenderSettings::EDisplay::Boundary:
            if (!Section.Boundary.empty())
            {
                return Section.Boundary;
            }
            break;
        case FSKEdgeRenderSettings::EDisplay::Sharp:
            if (!Section.Sharp.empty())
            {
                return Section.Sharp;
            }
            break;
        case FSKEdgeRenderSettings::EDisplay::All:
        default:
            break;
        }

        if (!Section.All.empty())
        {
            return Section.All;
        }
        if (!Section.Sharp.empty())
        {
            return Section.Sharp;
        }
        if (!Section.Boundary.empty())
        {
            return Section.Boundary;
        }

        static const TArray<FSKEdgePacketLine> GEmpty;
        return GEmpty;
    }

    const FSKEdgeDrawPacket& FSKEdgeDrawPacketCache::GetOrBuild(const FSKGeometryData& Geometry)
    {
        auto It = Packets.find(Geometry.GeometryKey);
        if (It != Packets.end())
        {
            return It->second;
        }

        FSKEdgeDrawPacket Packet = FSKEdgeDrawPacketBuilder::Build(Geometry);
        auto Result = Packets.emplace(Geometry.GeometryKey, std::move(Packet));
        return Result.first->second;
    }

    const FSKEdgeDrawPacket* FSKEdgeDrawPacketCache::Find(uint64 GeometryKey) const
    {
        auto It = Packets.find(GeometryKey);
        return (It != Packets.end()) ? &It->second : nullptr;
    }

    void FSKEdgeDrawPacketCache::Clear()
    {
        Packets.clear();
    }
}
