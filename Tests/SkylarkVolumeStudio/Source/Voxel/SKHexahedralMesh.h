#pragma once

#include "Voxel/SKVolumeVoxelGrid.h"

#include <array>

namespace Skylark::VolumeStudio
{
    struct FSKHexahedralCell
    {
        std::array<uint32, 8> NodeIds{};
        ESKVoxelCellState SourceState = ESKVoxelCellState::Empty;
    };

    class FSKHexahedralMesh final
    {
    public:
        TArray<FSKVector3f> Nodes;
        TArray<FSKHexahedralCell> Cells;

        void Reset();
        uint64 GetNodeCount() const { return static_cast<uint64>(Nodes.size()); }
        uint64 GetCellCount() const { return static_cast<uint64>(Cells.size()); }
        size_t EstimateMemoryBytes() const;
    };

    struct FSKHexahedralMeshBuildOutput
    {
        FSKVolumeStudioResult Result;
        FSKHexahedralMesh Mesh;
        FSKVolumeStudioBuildStats Stats;
    };

    class FSKHexahedralMeshBuilder final
    {
    public:
        static FSKHexahedralMeshBuildOutput BuildFromOccupiedVoxels(const FSKVolumeVoxelGrid& Grid);
    };
}
