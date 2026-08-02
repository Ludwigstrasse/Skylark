#pragma once

#include "Core/SKVolumeStudioMeshAsset.h"

namespace Skylark::VolumeStudio
{
    struct FSKStlFastReaderOptions
    {
        bool bStoreFacetNormals = false;
        bool bRejectNonFiniteCoordinates = true;
        uint64 MaxTriangleCount = 50ull * 1000ull * 1000ull;
        uint64 ReserveTriangleCountHint = 0;
    };

    struct FSKStlFastReaderOutput
    {
        FSKVolumeStudioResult Result;
        FSKVolumeStudioMeshAsset Mesh;
        uint64 FileBytes = 0;
        uint64 RejectedTriangleCount = 0;
    };

    class FSKStlFastReader final
    {
    public:
        static FSKStlFastReaderOutput ReadBinaryFile(const std::filesystem::path& FilePath, const FSKStlFastReaderOptions& Options = {});
        static bool IsBinaryStlLayout(uint64 FileBytes, uint32 TriangleCountFromHeader);
    };
}
