#include "SkylarkBridge/SKVolumeStudioSkylarkBridge.h"

#include <cmath>
#include <sstream>

namespace Skylark::VolumeStudio
{
    namespace
    {
        static FSKVector3f Cross(const FSKVector3f& A, const FSKVector3f& B)
        {
            return FSKVector3f(A.Y * B.Z - A.Z * B.Y,
                               A.Z * B.X - A.X * B.Z,
                               A.X * B.Y - A.Y * B.X);
        }

        static FSKVector3f Subtract(const FSKVector3f& A, const FSKVector3f& B)
        {
            return FSKVector3f(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
        }

        static FSKVector3f NormalizeOrFallback(const FSKVector3f& V)
        {
            const float Length = std::sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
            if (Length <= 1.0e-20f || !std::isfinite(Length))
            {
                return FSKVector3f(0.0f, 0.0f, 1.0f);
            }
            return FSKVector3f(V.X / Length, V.Y / Length, V.Z / Length);
        }

        static FSKVertex MakeVertex(const FSKVector3f& P, const FSKVector3f& N)
        {
            FSKVertex Vertex;
            Vertex.Position = P;
            Vertex.Normal = N;
            return Vertex;
        }
    }

    FSKVolumeStudioResult FSKVolumeStudioSkylarkBridge::BuildSurfacePreviewMesh(
        const FSKVolumeStudioMeshAsset& SourceMesh,
        const FSKVolumeStudioPreviewMeshOptions& Options,
        FSKMeshData& OutMesh)
    {
        OutMesh.Sections.clear();
        const uint64 SourceTriangleCount = SourceMesh.GetTriangleCount();
        if (SourceTriangleCount == 0)
        {
            return FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::EmptyInput, "Cannot build Skylark preview mesh from empty surface mesh.");
        }
        if (SourceTriangleCount > Options.MaxTriangles)
        {
            std::ostringstream Message;
            Message << "Preview mesh triangle count exceeds configured limit. source=" << SourceTriangleCount
                    << ", max=" << Options.MaxTriangles << ". Use streaming/GPU resident path for full model.";
            return FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::ResourceLimitExceeded, Message.str());
        }

        FSKMeshSection Section;
        Section.Vertices.reserve(static_cast<size_t>(SourceTriangleCount * 3ull));
        if (Options.bBuildIndices)
        {
            Section.Indices.reserve(static_cast<size_t>(SourceTriangleCount * 3ull));
        }

        for (uint64 TriangleIndex = 0; TriangleIndex < SourceTriangleCount; ++TriangleIndex)
        {
            const auto Triangle = SourceMesh.GetTriangle(TriangleIndex);
            const FSKVector3f Normal = NormalizeOrFallback(Cross(Subtract(Triangle[1], Triangle[0]), Subtract(Triangle[2], Triangle[0])));
            const uint32 BaseVertex = static_cast<uint32>(Section.Vertices.size());
            Section.Vertices.push_back(MakeVertex(Triangle[0], Normal));
            Section.Vertices.push_back(MakeVertex(Triangle[1], Normal));
            Section.Vertices.push_back(MakeVertex(Triangle[2], Normal));
            if (Options.bBuildIndices)
            {
                Section.Indices.push_back(BaseVertex + 0u);
                Section.Indices.push_back(BaseVertex + 1u);
                Section.Indices.push_back(BaseVertex + 2u);
            }
        }

        OutMesh.Sections.push_back(std::move(Section));
        return FSKVolumeStudioResult::Ok("Built Skylark surface preview mesh.");
    }

    FSKVolumeStudioResult FSKVolumeStudioSkylarkBridge::BuildHexCellCenterPointMesh(
        const FSKHexahedralMesh& HexMesh,
        uint64 MaxCells,
        FSKMeshData& OutMesh)
    {
        OutMesh.Sections.clear();
        if (HexMesh.Cells.empty())
        {
            return FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::EmptyInput, "Cannot build cell-center mesh from an empty hexahedral mesh.");
        }
        if (static_cast<uint64>(HexMesh.Cells.size()) > MaxCells)
        {
            return FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::ResourceLimitExceeded, "Hex mesh is too large for CPU preview point mesh.");
        }

        FSKMeshSection Section;
        Section.Vertices.reserve(HexMesh.Cells.size());
        for (const FSKHexahedralCell& Cell : HexMesh.Cells)
        {
            FSKVector3f Center(0.0f, 0.0f, 0.0f);
            for (uint32 NodeId : Cell.NodeIds)
            {
                const FSKVector3f& P = HexMesh.Nodes[NodeId];
                Center.X += P.X;
                Center.Y += P.Y;
                Center.Z += P.Z;
            }
            Center.X *= 0.125f;
            Center.Y *= 0.125f;
            Center.Z *= 0.125f;
            Section.Vertices.push_back(MakeVertex(Center, FSKVector3f(0.0f, 0.0f, 1.0f)));
        }

        OutMesh.Sections.push_back(std::move(Section));
        return FSKVolumeStudioResult::Ok("Built Skylark hex-cell center preview mesh.");
    }
}
