#include "Skylark/SkylarkMeshAdapter.h"

#include <algorithm>
#include <utility>

namespace Skylark::FormatWorkbench
{
#if SKFW_WITH_SKYLARK
    namespace
    {
        static FSKVector3f ToSkVector(const float* Ptr)
        {
            return FSKVector3f(Ptr[0], Ptr[1], Ptr[2]);
        }
    }

    FwSkylarkPreviewResult RegisterPreviewMeshInSkylark(const FwModelDocument& Document, Skylark::FSKGeometryRegistry& Registry, const FwSkylarkPreviewOptions& Options)
    {
        FwSkylarkPreviewResult Result;
        if (Document.TotalTriangles == 0)
        {
            Result.Message = "Document has no triangles.";
            return Result;
        }

        const std::uint64_t TriangleBudget = std::min(Document.TotalTriangles, Options.MaxTriangles);
        if (TriangleBudget == 0)
        {
            Result.Message = "Preview triangle budget is zero.";
            return Result;
        }

        FSKMeshData Mesh;
        std::uint64_t Remaining = TriangleBudget;
        for (const FwTriangleMeshBlock& SourceBlock : Document.MeshBlocks)
        {
            if (Remaining == 0)
                break;

            const std::uint64_t SourceTriangles = SourceBlock.Positions.size() / 9u;
            const std::uint32_t CopyTriangles = static_cast<std::uint32_t>(std::min<std::uint64_t>(SourceTriangles, Remaining));
            FSKMeshSection Section;
            Section.Vertices.reserve(static_cast<std::size_t>(CopyTriangles) * 3u);
            Section.Indices.reserve(static_cast<std::size_t>(CopyTriangles) * 3u);

            for (std::uint32_t I = 0; I < CopyTriangles; ++I)
            {
                const float* P = SourceBlock.Positions.data() + static_cast<std::size_t>(I) * 9u;
                FSKVertex V0{};
                FSKVertex V1{};
                FSKVertex V2{};
                V0.Position = ToSkVector(P + 0);
                V1.Position = ToSkVector(P + 3);
                V2.Position = ToSkVector(P + 6);
                const std::uint32_t BaseIndex = static_cast<std::uint32_t>(Section.Vertices.size());
                Section.Vertices.push_back(V0);
                Section.Vertices.push_back(V1);
                Section.Vertices.push_back(V2);
                Section.Indices.push_back(BaseIndex + 0u);
                Section.Indices.push_back(BaseIndex + 1u);
                Section.Indices.push_back(BaseIndex + 2u);
            }

            Mesh.Sections.push_back(std::move(Section));
            Remaining -= CopyTriangles;
        }

        FSKGeometryBuildSettings Settings;
        Settings.bBuildWireframe = Options.BuildWireframe;
        Settings.bBuildEdgeCategories = Options.BuildEdgeCategories;
        Result.GeometryKey = Registry.RegisterMesh(Document.SourcePath.filename().string(), Mesh, Settings);
        Result.UploadedTriangles = TriangleBudget - Remaining;
        Result.Succeeded = Result.GeometryKey != 0;
        Result.Message = Result.Succeeded
            ? "Registered preview mesh in FSKGeometryRegistry. Full 20M STL path must use compact GPU upload, not duplicated FSKVertex preview."
            : "FSKGeometryRegistry returned invalid key.";
        return Result;
    }
#else
    FwSkylarkPreviewResult RegisterPreviewMeshInSkylark(const FwModelDocument& Document, void*, const FwSkylarkPreviewOptions&)
    {
        FwSkylarkPreviewResult Result;
        Result.Succeeded = Document.TotalTriangles > 0;
        Result.UploadedTriangles = Document.TotalTriangles;
        Result.Message = "Skylark linkage is disabled; compact import path was validated only.";
        return Result;
    }
#endif
}
