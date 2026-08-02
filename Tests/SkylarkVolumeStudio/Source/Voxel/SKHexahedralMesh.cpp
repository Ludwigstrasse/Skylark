#include "Voxel/SKHexahedralMesh.h"

#include <chrono>
#include <sstream>
#include <unordered_map>

namespace Skylark::VolumeStudio
{
    namespace
    {
        static uint64 PackGridVertexKey(uint32 X, uint32 Y, uint32 Z)
        {
            // M1 safety limit is <= 256^3 cells, so 21 bits per axis is ample and portable.
            return (static_cast<uint64>(Z) << 42u) | (static_cast<uint64>(Y) << 21u) | static_cast<uint64>(X);
        }

        static uint32 GetOrCreateNode(
            const FSKVolumeVoxelGrid& Grid,
            uint32 X,
            uint32 Y,
            uint32 Z,
            std::unordered_map<uint64, uint32>& NodeMap,
            TArray<FSKVector3f>& Nodes)
        {
            const uint64 Key = PackGridVertexKey(X, Y, Z);
            const auto It = NodeMap.find(Key);
            if (It != NodeMap.end())
            {
                return It->second;
            }

            const uint32 NewId = static_cast<uint32>(Nodes.size());
            Nodes.push_back(Grid.GetGridVertexPosition(X, Y, Z));
            NodeMap.emplace(Key, NewId);
            return NewId;
        }
    }

    void FSKHexahedralMesh::Reset()
    {
        Nodes.clear();
        Cells.clear();
    }

    size_t FSKHexahedralMesh::EstimateMemoryBytes() const
    {
        return Nodes.size() * sizeof(FSKVector3f) + Cells.size() * sizeof(FSKHexahedralCell);
    }

    FSKHexahedralMeshBuildOutput FSKHexahedralMeshBuilder::BuildFromOccupiedVoxels(const FSKVolumeVoxelGrid& Grid)
    {
        const auto BeginTime = std::chrono::steady_clock::now();
        FSKHexahedralMeshBuildOutput Output;
        if (!Grid.IsValid())
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::InvalidArgument, "Invalid voxel grid for hexahedral mesh build.");
            return Output;
        }

        std::unordered_map<uint64, uint32> NodeMap;
        NodeMap.reserve(static_cast<size_t>(std::min<uint64>(Grid.GetCellCount() * 2ull, 4ull * 1000ull * 1000ull)));
        Output.Mesh.Cells.reserve(static_cast<size_t>(std::min<uint64>(Grid.GetCellCount(), 2ull * 1000ull * 1000ull)));

        for (uint32 Z = 0; Z < Grid.DimZ; ++Z)
        {
            for (uint32 Y = 0; Y < Grid.DimY; ++Y)
            {
                for (uint32 X = 0; X < Grid.DimX; ++X)
                {
                    const ESKVoxelCellState State = Grid.Get(X, Y, Z);
                    if (State == ESKVoxelCellState::Empty)
                    {
                        continue;
                    }

                    FSKHexahedralCell Cell;
                    Cell.SourceState = State;
                    Cell.NodeIds[0] = GetOrCreateNode(Grid, X + 0u, Y + 0u, Z + 0u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[1] = GetOrCreateNode(Grid, X + 1u, Y + 0u, Z + 0u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[2] = GetOrCreateNode(Grid, X + 1u, Y + 1u, Z + 0u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[3] = GetOrCreateNode(Grid, X + 0u, Y + 1u, Z + 0u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[4] = GetOrCreateNode(Grid, X + 0u, Y + 0u, Z + 1u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[5] = GetOrCreateNode(Grid, X + 1u, Y + 0u, Z + 1u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[6] = GetOrCreateNode(Grid, X + 1u, Y + 1u, Z + 1u, NodeMap, Output.Mesh.Nodes);
                    Cell.NodeIds[7] = GetOrCreateNode(Grid, X + 0u, Y + 1u, Z + 1u, NodeMap, Output.Mesh.Nodes);
                    Output.Mesh.Cells.push_back(Cell);
                }
            }
        }

        Output.Stats.OutputVertexCount = Output.Mesh.GetNodeCount();
        Output.Stats.OutputCellCount = Output.Mesh.GetCellCount();
        Output.Stats.MemoryBytes = Output.Mesh.EstimateMemoryBytes();
        const auto EndTime = std::chrono::steady_clock::now();
        Output.Stats.ElapsedSeconds = std::chrono::duration<double>(EndTime - BeginTime).count();

        std::ostringstream Message;
        Message << "Built hexahedral mesh. nodes=" << Output.Stats.OutputVertexCount
                << ", cells=" << Output.Stats.OutputCellCount << ".";
        Output.Result = FSKVolumeStudioResult::Ok(Message.str());
        return Output;
    }
}
