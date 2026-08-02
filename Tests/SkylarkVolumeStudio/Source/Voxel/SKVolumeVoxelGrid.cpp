#include "Voxel/SKVolumeVoxelGrid.h"

#include <chrono>
#include <cmath>
#include <sstream>

namespace Skylark::VolumeStudio
{
    namespace
    {
        static uint32 ClampToGridFloor(float Value, float Origin, float CellSize, uint32 Dim)
        {
            if (Dim == 0)
            {
                return 0;
            }
            const float Coord = std::floor((Value - Origin) / CellSize);
            if (Coord <= 0.0f)
            {
                return 0;
            }
            const auto Index = static_cast<uint64>(Coord);
            return static_cast<uint32>(std::min<uint64>(Index, static_cast<uint64>(Dim - 1)));
        }

        static uint32 ClampToGridCeil(float Value, float Origin, float CellSize, uint32 Dim)
        {
            if (Dim == 0)
            {
                return 0;
            }
            const float Coord = std::ceil((Value - Origin) / CellSize);
            if (Coord <= 0.0f)
            {
                return 0;
            }
            const auto Index = static_cast<uint64>(Coord);
            return static_cast<uint32>(std::min<uint64>(Index, static_cast<uint64>(Dim - 1)));
        }

        static float Max3(float X, float Y, float Z)
        {
            return std::max(X, std::max(Y, Z));
        }
    }

    void FSKVolumeVoxelGrid::Allocate(uint32 InDimX, uint32 InDimY, uint32 InDimZ, ESKVoxelCellState InitialState)
    {
        DimX = InDimX;
        DimY = InDimY;
        DimZ = InDimZ;
        const uint64 Count = static_cast<uint64>(DimX) * static_cast<uint64>(DimY) * static_cast<uint64>(DimZ);
        Cells.assign(static_cast<size_t>(Count), static_cast<uint8>(InitialState));
    }

    bool FSKVolumeVoxelGrid::IsValid() const
    {
        return DimX > 0 && DimY > 0 && DimZ > 0 && CellSize > 0.0f && Cells.size() == static_cast<size_t>(GetCellCount());
    }

    uint64 FSKVolumeVoxelGrid::GetCellCount() const
    {
        return static_cast<uint64>(DimX) * static_cast<uint64>(DimY) * static_cast<uint64>(DimZ);
    }

    size_t FSKVolumeVoxelGrid::EstimateMemoryBytes() const
    {
        return Cells.size() * sizeof(uint8);
    }

    bool FSKVolumeVoxelGrid::IsInside(uint32 X, uint32 Y, uint32 Z) const
    {
        return X < DimX && Y < DimY && Z < DimZ;
    }

    uint64 FSKVolumeVoxelGrid::GetLinearIndex(uint32 X, uint32 Y, uint32 Z) const
    {
        SK_CHECK(IsInside(X, Y, Z));
        return static_cast<uint64>(Z) * static_cast<uint64>(DimY) * static_cast<uint64>(DimX) +
               static_cast<uint64>(Y) * static_cast<uint64>(DimX) + static_cast<uint64>(X);
    }

    ESKVoxelCellState FSKVolumeVoxelGrid::Get(uint32 X, uint32 Y, uint32 Z) const
    {
        return static_cast<ESKVoxelCellState>(Cells[static_cast<size_t>(GetLinearIndex(X, Y, Z))]);
    }

    void FSKVolumeVoxelGrid::Set(uint32 X, uint32 Y, uint32 Z, ESKVoxelCellState State)
    {
        Cells[static_cast<size_t>(GetLinearIndex(X, Y, Z))] = static_cast<uint8>(State);
    }

    bool FSKVolumeVoxelGrid::IsOccupied(uint32 X, uint32 Y, uint32 Z) const
    {
        const ESKVoxelCellState State = Get(X, Y, Z);
        return State == ESKVoxelCellState::Boundary || State == ESKVoxelCellState::Solid;
    }

    FSKVector3f FSKVolumeVoxelGrid::GetCellMinCorner(uint32 X, uint32 Y, uint32 Z) const
    {
        return FSKVector3f(Origin.X + static_cast<float>(X) * CellSize,
                           Origin.Y + static_cast<float>(Y) * CellSize,
                           Origin.Z + static_cast<float>(Z) * CellSize);
    }

    FSKVector3f FSKVolumeVoxelGrid::GetGridVertexPosition(uint32 X, uint32 Y, uint32 Z) const
    {
        return FSKVector3f(Origin.X + static_cast<float>(X) * CellSize,
                           Origin.Y + static_cast<float>(Y) * CellSize,
                           Origin.Z + static_cast<float>(Z) * CellSize);
    }

    uint64 FSKVolumeVoxelGrid::CountCells(ESKVoxelCellState State) const
    {
        uint64 Count = 0;
        const uint8 Raw = static_cast<uint8>(State);
        for (uint8 Cell : Cells)
        {
            if (Cell == Raw)
            {
                ++Count;
            }
        }
        return Count;
    }

    FSKVoxelizationOutput FSKSurfaceVoxelizer::BuildConservativeVoxelGrid(const FSKVolumeStudioMeshAsset& Mesh, const FSKVoxelBuildSettings& Settings)
    {
        const auto BeginTime = std::chrono::steady_clock::now();
        FSKVoxelizationOutput Output;
        Output.Stats.InputTriangleCount = Mesh.GetTriangleCount();

        if (Mesh.GetTriangleCount() == 0 || !Mesh.GetBounds().IsValid())
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::EmptyInput, "Voxelization input mesh is empty.");
            return Output;
        }

        const FSKVector3f Extent = Mesh.GetBounds().Extent();
        const float LongestAxis = Max3(Extent.X, Extent.Y, Extent.Z);
        if (!(LongestAxis > 0.0f))
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::InvalidArgument, "Voxelization input bounds are degenerate.");
            return Output;
        }

        const uint32 MaxResolution = std::max<uint32>(Settings.MaxResolutionPerAxis, 1u);
        const float RequestedCellSize = Settings.CellSize;
        float CellSize = RequestedCellSize > 0.0f ? RequestedCellSize : LongestAxis / static_cast<float>(MaxResolution);
        if (!(CellSize > 0.0f) || !std::isfinite(CellSize))
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::InvalidArgument, "Invalid voxel cell size.");
            return Output;
        }

        const float Padding = std::max(0.0f, Settings.BoundsPaddingRatio) * LongestAxis + CellSize;
        Output.Grid.Origin = FSKVector3f(Mesh.GetBounds().Min.X - Padding, Mesh.GetBounds().Min.Y - Padding, Mesh.GetBounds().Min.Z - Padding);
        Output.Grid.CellSize = CellSize;

        const uint32 DimX = static_cast<uint32>(std::ceil((Extent.X + 2.0f * Padding) / CellSize)) + 1u;
        const uint32 DimY = static_cast<uint32>(std::ceil((Extent.Y + 2.0f * Padding) / CellSize)) + 1u;
        const uint32 DimZ = static_cast<uint32>(std::ceil((Extent.Z + 2.0f * Padding) / CellSize)) + 1u;

        const uint64 CellCount = static_cast<uint64>(DimX) * static_cast<uint64>(DimY) * static_cast<uint64>(DimZ);
        constexpr uint64 MaxM1CellCount = 256ull * 256ull * 256ull;
        if (CellCount > MaxM1CellCount)
        {
            std::ostringstream Message;
            Message << "Requested voxel grid exceeds M1 safety limit. cells=" << CellCount << ", limit=" << MaxM1CellCount << ".";
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::ResourceLimitExceeded, Message.str());
            return Output;
        }

        Output.Grid.Allocate(DimX, DimY, DimZ);

        for (uint64 TriangleIndex = 0; TriangleIndex < Mesh.GetTriangleCount(); ++TriangleIndex)
        {
            const auto Triangle = Mesh.GetTriangle(TriangleIndex);
            const FSKVector3f TriMin = SKVSMin(SKVSMin(Triangle[0], Triangle[1]), Triangle[2]);
            const FSKVector3f TriMax = SKVSMax(SKVSMax(Triangle[0], Triangle[1]), Triangle[2]);

            const uint32 X0 = ClampToGridFloor(TriMin.X, Output.Grid.Origin.X, CellSize, DimX);
            const uint32 Y0 = ClampToGridFloor(TriMin.Y, Output.Grid.Origin.Y, CellSize, DimY);
            const uint32 Z0 = ClampToGridFloor(TriMin.Z, Output.Grid.Origin.Z, CellSize, DimZ);
            const uint32 X1 = ClampToGridCeil(TriMax.X, Output.Grid.Origin.X, CellSize, DimX);
            const uint32 Y1 = ClampToGridCeil(TriMax.Y, Output.Grid.Origin.Y, CellSize, DimY);
            const uint32 Z1 = ClampToGridCeil(TriMax.Z, Output.Grid.Origin.Z, CellSize, DimZ);

            for (uint32 Z = Z0; Z <= Z1; ++Z)
            {
                for (uint32 Y = Y0; Y <= Y1; ++Y)
                {
                    for (uint32 X = X0; X <= X1; ++X)
                    {
                        Output.Grid.Set(X, Y, Z, ESKVoxelCellState::Boundary);
                    }
                }
            }
        }

        if (Settings.bFillInteriorByAxisParity)
        {
            FillInteriorByAxisParity(Output.Grid);
        }

        Output.Stats.BoundaryCellCount = Output.Grid.CountCells(ESKVoxelCellState::Boundary);
        Output.Stats.SolidCellCount = Output.Grid.CountCells(ESKVoxelCellState::Solid);
        Output.Stats.OutputCellCount = Output.Stats.BoundaryCellCount + Output.Stats.SolidCellCount;
        Output.Stats.MemoryBytes = Output.Grid.EstimateMemoryBytes();
        const auto EndTime = std::chrono::steady_clock::now();
        Output.Stats.ElapsedSeconds = std::chrono::duration<double>(EndTime - BeginTime).count();

        std::ostringstream Message;
        Message << "Built conservative voxel grid. dims=" << Output.Grid.DimX << "x" << Output.Grid.DimY << "x" << Output.Grid.DimZ
                << ", boundary=" << Output.Stats.BoundaryCellCount
                << ", solid=" << Output.Stats.SolidCellCount << ".";
        Output.Result = FSKVolumeStudioResult::Ok(Message.str());
        return Output;
    }

    void FSKSurfaceVoxelizer::FillInteriorByAxisParity(FSKVolumeVoxelGrid& Grid)
    {
        if (!Grid.IsValid())
        {
            return;
        }

        for (uint32 Z = 0; Z < Grid.DimZ; ++Z)
        {
            for (uint32 Y = 0; Y < Grid.DimY; ++Y)
            {
                bool bInside = false;
                bool bPreviousBoundary = false;
                for (uint32 X = 0; X < Grid.DimX; ++X)
                {
                    const bool bBoundary = Grid.Get(X, Y, Z) == ESKVoxelCellState::Boundary;
                    if (bBoundary)
                    {
                        if (!bPreviousBoundary)
                        {
                            bInside = !bInside;
                        }
                        bPreviousBoundary = true;
                        continue;
                    }

                    bPreviousBoundary = false;
                    if (bInside)
                    {
                        Grid.Set(X, Y, Z, ESKVoxelCellState::Solid);
                    }
                }
            }
        }
    }
}
