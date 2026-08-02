#include "SKRenderer/SKGpuResidentBufferCache.h"

#include "SKRenderer/SKEdgeDrawPacket.h"
#include "SKRenderer/SKMeshDrawPacket.h"

namespace Skylark
{
    namespace
    {
        static uint32 PackRGBA8(uint8 R, uint8 G, uint8 B, uint8 A)
        {
            return (uint32)R | ((uint32)G << 8u) | ((uint32)B << 16u) | ((uint32)A << 24u);
        }
    }

    namespace
    {
        static FSKRHIResidentTriangleBufferDesc MakeTriangleDesc(const FSKMeshSectionDrawPacket& Section, TArray<FSKRHIResidentTriangleVertex>& OutVertices, TArray<uint32>& OutIndices)
        {
            OutVertices.clear();
            OutIndices.clear();
            OutVertices.reserve(Section.Vertices.size());
            OutIndices.reserve(Section.Indices.size());

            for (const auto& V : Section.Vertices)
            {
                FSKRHIResidentTriangleVertex RV;
                RV.PX = V.PositionLocal.X;
                RV.PY = V.PositionLocal.Y;
                RV.PZ = V.PositionLocal.Z;
                RV.NX = V.NormalLocal.X;
                RV.NY = V.NormalLocal.Y;
                RV.NZ = V.NormalLocal.Z;
                RV.ColorRGBA8 = PackRGBA8(
                    (uint8)std::clamp((int)(Section.BaseColor.X * 255.0f), 0, 255),
                    (uint8)std::clamp((int)(Section.BaseColor.Y * 255.0f), 0, 255),
                    (uint8)std::clamp((int)(Section.BaseColor.Z * 255.0f), 0, 255),
                    255u);
                OutVertices.push_back(RV);
            }
            for (uint32 I : Section.Indices)
            {
                if (I < OutVertices.size())
                {
                    OutIndices.push_back(I);
                }
            }

            FSKRHIResidentTriangleBufferDesc Desc;
            Desc.Vertices = OutVertices.data();
            Desc.VertexCount = static_cast<uint32>(OutVertices.size());
            Desc.Indices = OutIndices.data();
            Desc.IndexCount = static_cast<uint32>(OutIndices.size());
            Desc.DebugName = "SkylarkResidentMeshSection";
            return Desc;
        }

        static std::unique_ptr<ISKRHIResidentLineBuffer> BuildLineBuffer(ISKRHICommandList& Cmd, const TArray<FSKEdgePacketLine>& Lines, const char* DebugName, SIZE_T& OutVertexCount)
        {
            OutVertexCount = 0;
            if (Lines.empty())
            {
                return {};
            }

            TArray<FSKRHIResidentLineVertex> Vertices;
            Vertices.reserve(Lines.size() * 2);

            for (const auto& Line : Lines)
            {
                FSKRHIResidentLineVertex V0;
                V0.PX = Line.P0Local.X; V0.PY = Line.P0Local.Y; V0.PZ = Line.P0Local.Z;
                V0.ColorRGBA8 = 0xFFFFFFFFu;
                FSKRHIResidentLineVertex V1;
                V1.PX = Line.P1Local.X; V1.PY = Line.P1Local.Y; V1.PZ = Line.P1Local.Z;
                V1.ColorRGBA8 = 0xFFFFFFFFu;
                Vertices.push_back(V0);
                Vertices.push_back(V1);
            }

            FSKRHIResidentLineBufferDesc Desc;
            Desc.Vertices = Vertices.data();
            Desc.VertexCount = static_cast<uint32>(Vertices.size());
            Desc.DebugName = DebugName;
            OutVertexCount = Vertices.size();
            return Cmd.CreateResidentLineBuffer(Desc);
        }
    }

    FSKGpuResidentBufferCache& FSKGpuResidentBufferCache::Get()
    {
        static FSKGpuResidentBufferCache GCache;
        return GCache;
    }

    const FSKResidentMeshGeometryBuffers& FSKGpuResidentBufferCache::GetOrCreateMeshBuffers(ISKRHICommandList& Cmd, const FSKGeometryData& Geometry)
    {
        auto It = MeshBuffers.find(Geometry.GeometryKey);
        if (It != MeshBuffers.end())
        {
            return It->second;
        }

        FSKResidentMeshGeometryBuffers Entry;
        Entry.GeometryKey = Geometry.GeometryKey;
        Entry.DebugName = Geometry.DebugName;

        const FSKMeshDrawPacket& Packet = FSKGeometryResidencyCache::Get().GetOrBuildMeshPacket(Geometry);
        Entry.Sections.reserve(Packet.Sections.size());

        TArray<FSKRHIResidentTriangleVertex> TempVertices;
        TArray<uint32> TempIndices;
        for (const auto& Section : Packet.Sections)
        {
            FSKResidentMeshSectionBuffers ResidentSection;
            ResidentSection.OwnerId = Section.OwnerId;
            ResidentSection.SectionHitProxyId = Section.SectionHitProxyId;
            ResidentSection.BaseColor = Section.BaseColor;

            FSKRHIResidentTriangleBufferDesc Desc = MakeTriangleDesc(Section, TempVertices, TempIndices);
            if (Desc.Vertices && Desc.Indices && Desc.VertexCount > 0 && Desc.IndexCount > 0)
            {
                ResidentSection.Buffer = Cmd.CreateResidentTriangleBuffer(Desc);
                if (ResidentSection.Buffer)
                {
                    Entry.bResidentSupported = true;
                }
            }
            Entry.Sections.push_back(std::move(ResidentSection));
        }

        auto Result = MeshBuffers.emplace(Geometry.GeometryKey, std::move(Entry));
        RebuildStats();
        return Result.first->second;
    }

    const FSKResidentEdgeGeometryBuffers& FSKGpuResidentBufferCache::GetOrCreateEdgeBuffers(ISKRHICommandList& Cmd, const FSKGeometryData& Geometry)
    {
        auto It = EdgeBuffers.find(Geometry.GeometryKey);
        if (It != EdgeBuffers.end())
        {
            return It->second;
        }

        FSKResidentEdgeGeometryBuffers Entry;
        Entry.GeometryKey = Geometry.GeometryKey;
        Entry.DebugName = Geometry.DebugName;

        const FSKEdgeDrawPacket& Packet = FSKGeometryResidencyCache::Get().GetOrBuildEdgePacket(Geometry);
        Entry.Sections.reserve(Packet.Sections.size());

        for (const auto& Section : Packet.Sections)
        {
            FSKResidentEdgeSectionBuffers ResidentSection;
            ResidentSection.OwnerId = Section.OwnerId;
            ResidentSection.SectionHitProxyId = Section.SectionHitProxyId;

            SIZE_T VertexCount = 0;
            ResidentSection.All = BuildLineBuffer(Cmd, Section.All, "SkylarkResidentAllEdges", VertexCount);
            if (ResidentSection.All) Entry.bResidentSupported = true;

            ResidentSection.Boundary = BuildLineBuffer(Cmd, Section.Boundary, "SkylarkResidentBoundaryEdges", VertexCount);
            if (ResidentSection.Boundary) Entry.bResidentSupported = true;

            ResidentSection.Sharp = BuildLineBuffer(Cmd, Section.Sharp, "SkylarkResidentSharpEdges", VertexCount);
            if (ResidentSection.Sharp) Entry.bResidentSupported = true;

            Entry.Sections.push_back(std::move(ResidentSection));
        }

        auto Result = EdgeBuffers.emplace(Geometry.GeometryKey, std::move(Entry));
        RebuildStats();
        return Result.first->second;
    }

    void FSKGpuResidentBufferCache::Clear()
    {
        MeshBuffers.clear();
        EdgeBuffers.clear();
        Stats = {};
    }

    const FSKGpuResidentBufferStats& FSKGpuResidentBufferCache::GetStats() const
    {
        return Stats;
    }

    void FSKGpuResidentBufferCache::RebuildStats()
    {
        Stats = {};
        Stats.MeshGeometryCount = MeshBuffers.size();
        Stats.EdgeGeometryCount = EdgeBuffers.size();

        for (const auto& KV : MeshBuffers)
        {
            for (const auto& Section : KV.second.Sections)
            {
                if (!Section.Buffer)
                {
                    continue;
                }
                ++Stats.ResidentMeshSectionCount;
                Stats.ResidentTriangleVertexCount += Section.Buffer->GetVertexCount();
                Stats.ResidentTriangleIndexCount += Section.Buffer->GetIndexCount();
            }
        }

        for (const auto& KV : EdgeBuffers)
        {
            for (const auto& Section : KV.second.Sections)
            {
                bool bSectionResident = false;
                if (Section.All) { Stats.ResidentLineVertexCount += Section.All->GetVertexCount(); bSectionResident = true; }
                if (Section.Boundary) { Stats.ResidentLineVertexCount += Section.Boundary->GetVertexCount(); bSectionResident = true; }
                if (Section.Sharp) { Stats.ResidentLineVertexCount += Section.Sharp->GetVertexCount(); bSectionResident = true; }
                if (bSectionResident)
                {
                    ++Stats.ResidentEdgeSectionCount;
                }
            }
        }
    }
}
