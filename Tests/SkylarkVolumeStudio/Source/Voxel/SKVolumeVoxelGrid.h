#pragma once

#include "Core/SKVolumeStudioMeshAsset.h"

namespace Skylark::VolumeStudio
{
    enum class ESKVoxelCellState : uint8
    {
        Empty = 0,
        Boundary = 1,
        Solid = 2
    };

    struct FSKVoxelBuildSettings
    {
        float CellSize = 0.0f;
        uint32 MaxResolutionPerAxis = 128;
        float BoundsPaddingRatio = 0.005f;
        bool bMarkTriangleAabbConservatively = true;
        bool bFillInteriorByAxisParity = true;
    };

    class FSKVolumeVoxelGrid final
    {
    public:
        FSKVector3f Origin{0.0f, 0.0f, 0.0f};
        float CellSize = 1.0f;
        uint32 DimX = 0;
        uint32 DimY = 0;
        uint32 DimZ = 0;

        void Allocate(uint32 InDimX, uint32 InDimY, uint32 InDimZ, ESKVoxelCellState InitialState = ESKVoxelCellState::Empty);
        bool IsValid() const;
        uint64 GetCellCount() const;
        size_t EstimateMemoryBytes() const;

        bool IsInside(uint32 X, uint32 Y, uint32 Z) const;
        uint64 GetLinearIndex(uint32 X, uint32 Y, uint32 Z) const;
        ESKVoxelCellState Get(uint32 X, uint32 Y, uint32 Z) const;
        void Set(uint32 X, uint32 Y, uint32 Z, ESKVoxelCellState State);
        bool IsOccupied(uint32 X, uint32 Y, uint32 Z) const;
        FSKVector3f GetCellMinCorner(uint32 X, uint32 Y, uint32 Z) const;
        FSKVector3f GetGridVertexPosition(uint32 X, uint32 Y, uint32 Z) const;

        uint64 CountCells(ESKVoxelCellState State) const;
        const TArray<uint8>& GetCells() const { return Cells; }

    private:
        TArray<uint8> Cells;
    };

    struct FSKVoxelizationOutput
    {
        FSKVolumeStudioResult Result;
        FSKVolumeVoxelGrid Grid;
        FSKVolumeStudioBuildStats Stats;
    };

    class FSKSurfaceVoxelizer final
    {
    public:
        static FSKVoxelizationOutput BuildConservativeVoxelGrid(const FSKVolumeStudioMeshAsset& Mesh, const FSKVoxelBuildSettings& Settings = {});

    private:
        static void FillInteriorByAxisParity(FSKVolumeVoxelGrid& Grid);
    };
}
