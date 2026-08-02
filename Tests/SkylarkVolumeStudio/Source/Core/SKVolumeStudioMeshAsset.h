#pragma once

#include "Core/SKVolumeStudioTypes.h"

namespace Skylark::VolumeStudio
{
    class FSKVolumeStudioMeshAsset final
    {
    public:
        void Reset();
        void ReserveTriangles(uint64 TriangleCount, bool bStoreFacetNormals);
        bool AppendTriangle(const FSKVector3f& P0, const FSKVector3f& P1, const FSKVector3f& P2, const FSKVector3f* FacetNormal = nullptr);

        uint64 GetTriangleCount() const;
        bool HasFacetNormals() const;
        size_t EstimateMemoryBytes() const;

        const TArray<float>& GetPositionBuffer() const { return PositionBuffer; }
        const TArray<float>& GetFacetNormalBuffer() const { return FacetNormalBuffer; }
        const FSKVolumeStudioAabb& GetBounds() const { return Bounds; }

        FSKVector3f GetTrianglePoint(uint64 TriangleIndex, uint32 LocalVertexIndex) const;
        std::array<FSKVector3f, 3> GetTriangle(uint64 TriangleIndex) const;

    private:
        TArray<float> PositionBuffer;
        TArray<float> FacetNormalBuffer;
        FSKVolumeStudioAabb Bounds;
    };
}
