#pragma once

#include "Core/SKVolumeStudioMeshAsset.h"
#include "Voxel/SKHexahedralMesh.h"
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark::VolumeStudio
{
    struct FSKVolumeStudioPreviewMeshOptions
    {
        uint64 MaxTriangles = 2ull * 1000ull * 1000ull;
        bool bBuildIndices = true;
    };

    class FSKVolumeStudioSkylarkBridge final
    {
    public:
        static FSKVolumeStudioResult BuildSurfacePreviewMesh(
            const FSKVolumeStudioMeshAsset& SourceMesh,
            const FSKVolumeStudioPreviewMeshOptions& Options,
            FSKMeshData& OutMesh);

        static FSKVolumeStudioResult BuildHexCellCenterPointMesh(
            const FSKHexahedralMesh& HexMesh,
            uint64 MaxCells,
            FSKMeshData& OutMesh);
    };
}
