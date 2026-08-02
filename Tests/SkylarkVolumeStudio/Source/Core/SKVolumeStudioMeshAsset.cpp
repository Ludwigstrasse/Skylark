#include "Core/SKVolumeStudioMeshAsset.h"

namespace Skylark::VolumeStudio
{
    void FSKVolumeStudioMeshAsset::Reset()
    {
        PositionBuffer.clear();
        FacetNormalBuffer.clear();
        Bounds = FSKVolumeStudioAabb{};
    }

    void FSKVolumeStudioMeshAsset::ReserveTriangles(uint64 TriangleCount, bool bStoreFacetNormals)
    {
        constexpr uint64 FloatsPerTriangle = 9;
        if (TriangleCount > static_cast<uint64>(std::numeric_limits<size_t>::max() / sizeof(float) / FloatsPerTriangle))
        {
            return;
        }

        PositionBuffer.reserve(static_cast<size_t>(TriangleCount * FloatsPerTriangle));
        if (bStoreFacetNormals)
        {
            FacetNormalBuffer.reserve(static_cast<size_t>(TriangleCount * 3));
        }
    }

    bool FSKVolumeStudioMeshAsset::AppendTriangle(
        const FSKVector3f& P0,
        const FSKVector3f& P1,
        const FSKVector3f& P2,
        const FSKVector3f* FacetNormal)
    {
        if (!SKVSIsFinite(P0) || !SKVSIsFinite(P1) || !SKVSIsFinite(P2))
        {
            return false;
        }

        PositionBuffer.push_back(P0.X);
        PositionBuffer.push_back(P0.Y);
        PositionBuffer.push_back(P0.Z);
        PositionBuffer.push_back(P1.X);
        PositionBuffer.push_back(P1.Y);
        PositionBuffer.push_back(P1.Z);
        PositionBuffer.push_back(P2.X);
        PositionBuffer.push_back(P2.Y);
        PositionBuffer.push_back(P2.Z);

        Bounds.Encapsulate(P0);
        Bounds.Encapsulate(P1);
        Bounds.Encapsulate(P2);

        if (FacetNormal != nullptr)
        {
            FacetNormalBuffer.push_back(FacetNormal->X);
            FacetNormalBuffer.push_back(FacetNormal->Y);
            FacetNormalBuffer.push_back(FacetNormal->Z);
        }

        return true;
    }

    uint64 FSKVolumeStudioMeshAsset::GetTriangleCount() const
    {
        return static_cast<uint64>(PositionBuffer.size() / 9);
    }

    bool FSKVolumeStudioMeshAsset::HasFacetNormals() const
    {
        return !FacetNormalBuffer.empty();
    }

    size_t FSKVolumeStudioMeshAsset::EstimateMemoryBytes() const
    {
        return PositionBuffer.size() * sizeof(float) + FacetNormalBuffer.size() * sizeof(float);
    }

    FSKVector3f FSKVolumeStudioMeshAsset::GetTrianglePoint(uint64 TriangleIndex, uint32 LocalVertexIndex) const
    {
        const uint64 Offset = TriangleIndex * 9 + static_cast<uint64>(LocalVertexIndex) * 3;
        SK_CHECK(LocalVertexIndex < 3);
        SK_CHECK(Offset + 2 < PositionBuffer.size());
        return FSKVector3f(PositionBuffer[static_cast<size_t>(Offset + 0)],
                           PositionBuffer[static_cast<size_t>(Offset + 1)],
                           PositionBuffer[static_cast<size_t>(Offset + 2)]);
    }

    std::array<FSKVector3f, 3> FSKVolumeStudioMeshAsset::GetTriangle(uint64 TriangleIndex) const
    {
        return {GetTrianglePoint(TriangleIndex, 0), GetTrianglePoint(TriangleIndex, 1), GetTrianglePoint(TriangleIndex, 2)};
    }
}
