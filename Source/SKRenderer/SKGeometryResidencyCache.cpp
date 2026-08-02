#include "SKRenderer/SKGeometryResidencyCache.h"

namespace Skylark
{
    FSKGeometryResidencyCache& FSKGeometryResidencyCache::Get()
    {
        static FSKGeometryResidencyCache GCache;
        return GCache;
    }

    const FSKMeshDrawPacket& FSKGeometryResidencyCache::GetOrBuildMeshPacket(const FSKGeometryData& Geometry)
    {
        auto It = MeshPackets.find(Geometry.GeometryKey);
        if (It != MeshPackets.end())
        {
            ++Stats.MeshPacketHits;
            return It->second;
        }

        ++Stats.MeshPacketMisses;
        FSKMeshDrawPacket Packet = FSKMeshDrawPacketBuilder::Build(Geometry);
        auto Result = MeshPackets.emplace(Geometry.GeometryKey, std::move(Packet));
        RebuildStats();
        return Result.first->second;
    }

    const FSKEdgeDrawPacket& FSKGeometryResidencyCache::GetOrBuildEdgePacket(const FSKGeometryData& Geometry)
    {
        auto It = EdgePackets.find(Geometry.GeometryKey);
        if (It != EdgePackets.end())
        {
            ++Stats.EdgePacketHits;
            return It->second;
        }

        ++Stats.EdgePacketMisses;
        FSKEdgeDrawPacket Packet = FSKEdgeDrawPacketBuilder::Build(Geometry);
        auto Result = EdgePackets.emplace(Geometry.GeometryKey, std::move(Packet));
        RebuildStats();
        return Result.first->second;
    }

    void FSKGeometryResidencyCache::Clear()
    {
        MeshPackets.clear();
        EdgePackets.clear();
        Stats = {};
    }

    const FSKGeometryResidencyStats& FSKGeometryResidencyCache::GetStats() const
    {
        return Stats;
    }

    void FSKGeometryResidencyCache::RebuildStats()
    {
        Stats.ResidentMeshGeometryCount = MeshPackets.size();
        Stats.ResidentEdgeGeometryCount = EdgePackets.size();
        Stats.ResidentMeshVertexCount = 0;
        Stats.ResidentMeshIndexCount = 0;
        Stats.ResidentEdgeLineCount = 0;

        for (const auto& KV : MeshPackets)
        {
            Stats.ResidentMeshVertexCount += KV.second.TotalVertexCount;
            Stats.ResidentMeshIndexCount += KV.second.TotalIndexCount;
        }
        for (const auto& KV : EdgePackets)
        {
            Stats.ResidentEdgeLineCount += KV.second.TotalAllLineCount;
        }
    }
}
