#include "SkCadSceneImporter.h"
#include "SkCadSceneFile.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <set>

namespace Skylark::CadSceneTest
{
    namespace
    {
        FAppVec3 MakeVec3(float X, float Y, float Z)
        {
            return FAppVec3{ X, Y, Z };
        }

        FAppVec3 Cross(const FAppVec3& A, const FAppVec3& B)
        {
            return FAppVec3{
                A.Y * B.Z - A.Z * B.Y,
                A.Z * B.X - A.X * B.Z,
                A.X * B.Y - A.Y * B.X
            };
        }

        float Dot(const FAppVec3& A, const FAppVec3& B)
        {
            return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
        }

        FAppVec3 TransformPoint(const FAppMat4& M, const FAppVec3& P)
        {
            FAppVec3 Out{};
            Out.X = M.M[0] * P.X + M.M[4] * P.Y + M.M[8]  * P.Z + M.M[12];
            Out.Y = M.M[1] * P.X + M.M[5] * P.Y + M.M[9]  * P.Z + M.M[13];
            Out.Z = M.M[2] * P.X + M.M[6] * P.Y + M.M[10] * P.Z + M.M[14];
            return Out;
        }

        FAppMat4 LoadMatrixColumnMajor(const float* pValues)
        {
            FAppMat4 Result{};
            std::memcpy(Result.M, pValues, sizeof(float) * 16);
            return Result;
        }

        void ExpandBounds(FAppVec3& InOutMin, FAppVec3& InOutMax, const FAppVec3& P)
        {
            InOutMin.X = std::min(InOutMin.X, P.X);
            InOutMin.Y = std::min(InOutMin.Y, P.Y);
            InOutMin.Z = std::min(InOutMin.Z, P.Z);
            InOutMax.X = std::max(InOutMax.X, P.X);
            InOutMax.Y = std::max(InOutMax.Y, P.Y);
            InOutMax.Z = std::max(InOutMax.Z, P.Z);
        }
    }

    FAppMat4 AppMat4Identity()
    {
        FAppMat4 Result{};
        Result.M[0] = 1.0f;
        Result.M[5] = 1.0f;
        Result.M[10] = 1.0f;
        Result.M[15] = 1.0f;
        return Result;
    }

    FAppMat4 AppMat4Translation(float X, float Y, float Z)
    {
        FAppMat4 Result = AppMat4Identity();
        Result.M[12] = X;
        Result.M[13] = Y;
        Result.M[14] = Z;
        return Result;
    }

    FAppMat4 AppMat4Multiply(const FAppMat4& A, const FAppMat4& B)
    {
        FAppMat4 Result{};
        for (int Column = 0; Column < 4; ++Column)
        {
            for (int Row = 0; Row < 4; ++Row)
            {
                Result.M[Column * 4 + Row] =
                    A.M[0 * 4 + Row] * B.M[Column * 4 + 0] +
                    A.M[1 * 4 + Row] * B.M[Column * 4 + 1] +
                    A.M[2 * 4 + Row] * B.M[Column * 4 + 2] +
                    A.M[3 * 4 + Row] * B.M[Column * 4 + 3];
            }
        }
        return Result;
    }

    FAppMat4 AppMat4Perspective(float fovyRadians, float aspect, float zNear, float zFar)
    {
        const float TanHalf = std::tan(fovyRadians * 0.5f);
        FAppMat4 Result{};
        Result.M[0] = 1.0f / (aspect * TanHalf);
        Result.M[5] = 1.0f / TanHalf;
        Result.M[10] = -(zFar + zNear) / (zFar - zNear);
        Result.M[11] = -1.0f;
        Result.M[14] = -(2.0f * zFar * zNear) / (zFar - zNear);
        return Result;
    }

    FAppMat4 AppMat4LookAt(const FAppVec3& Eye, const FAppVec3& Center, const FAppVec3& Up)
    {
        const FAppVec3 F = AppVec3Normalize(AppVec3Sub(Center, Eye));
        const FAppVec3 S = AppVec3Normalize(Cross(F, Up));
        const FAppVec3 U = Cross(S, F);

        FAppMat4 Result = AppMat4Identity();
        Result.M[0] = S.X; Result.M[4] = S.Y; Result.M[8]  = S.Z;
        Result.M[1] = U.X; Result.M[5] = U.Y; Result.M[9]  = U.Z;
        Result.M[2] = -F.X; Result.M[6] = -F.Y; Result.M[10] = -F.Z;
        Result.M[12] = -Dot(S, Eye);
        Result.M[13] = -Dot(U, Eye);
        Result.M[14] =  Dot(F, Eye);
        return Result;
    }

    FAppVec3 AppVec3Add(const FAppVec3& A, const FAppVec3& B) { return MakeVec3(A.X + B.X, A.Y + B.Y, A.Z + B.Z); }
    FAppVec3 AppVec3Sub(const FAppVec3& A, const FAppVec3& B) { return MakeVec3(A.X - B.X, A.Y - B.Y, A.Z - B.Z); }
    FAppVec3 AppVec3Scale(const FAppVec3& V, float S) { return MakeVec3(V.X * S, V.Y * S, V.Z * S); }
    float AppVec3Length(const FAppVec3& V) { return std::sqrt(Dot(V, V)); }

    FAppVec3 AppVec3Normalize(const FAppVec3& V)
    {
        const float Length = AppVec3Length(V);
        if (Length <= 1e-6f)
        {
            return MakeVec3(0.0f, 0.0f, 1.0f);
        }
        return MakeVec3(V.X / Length, V.Y / Length, V.Z / Length);
    }

    bool LoadCadSceneDocument(const char* pFilename, FSkCadSceneDocument& OutDocument, FSKString& OutError)
    {
        OutDocument = {};
        OutDocument.SceneMin = MakeVec3(+INFINITY, +INFINITY, +INFINITY);
        OutDocument.SceneMax = MakeVec3(-INFINITY, -INFINITY, -INFINITY);

        CSFile* pFile = nullptr;
        CSFileMemoryPTR pMemory = CSFileMemory_new();
        if (!pMemory)
        {
            OutError = "CSF memory allocator creation failed.";
            return false;
        }

        const int LoadResult = CSFile_loadExt(&pFile, pFilename, pMemory);
        if (LoadResult != CADSCENEFILE_NOERROR || !pFile)
        {
            CSFileMemory_delete(pMemory);
            OutError = "Failed to load CSF file.";
            return false;
        }

        if ((pFile->fileFlags & CADSCENEFILE_FLAG_UNIQUENODES) == 0)
        {
            CSFileMemory_delete(pMemory);
            OutError = "Only unique-node CSF scenes are supported by this Skylark test port.";
            return false;
        }

        CSFile_transform(pFile);

        OutDocument.Materials.reserve(static_cast<SIZE_T>(pFile->numMaterials));
        for (int32 MaterialIndex = 0; MaterialIndex < pFile->numMaterials; ++MaterialIndex)
        {
            const CSFMaterial& Material = pFile->materials[MaterialIndex];
            FSkCadSceneMaterial NewMaterial{};
            NewMaterial.Name = Material.name;
            NewMaterial.BaseColor = FAppVec4{ Material.color[0], Material.color[1], Material.color[2], Material.color[3] };
            OutDocument.Materials.push_back(std::move(NewMaterial));
        }

        TArray<uint64> GeometryKeys;
        GeometryKeys.resize(static_cast<SIZE_T>(pFile->numGeometries));
        TArray<FSKAabb> GeometryBounds;
        GeometryBounds.resize(static_cast<SIZE_T>(pFile->numGeometries));

        for (int32 GeometryIndex = 0; GeometryIndex < pFile->numGeometries; ++GeometryIndex)
        {
            const CSFGeometry& Geometry = pFile->geometries[GeometryIndex];
            FSKMeshData Mesh{};
            Mesh.Sections.reserve(static_cast<SIZE_T>(Geometry.numParts));

            int32 SolidIndexOffset = 0;
            for (int32 PartIndex = 0; PartIndex < Geometry.numParts; ++PartIndex)
            {
                const CSFGeometryPart& Part = Geometry.parts[PartIndex];
                FSKMeshSection Section{};
                Section.OwnerId.LegacyValue = static_cast<uint64>((static_cast<uint64>(GeometryIndex) << 32) | static_cast<uint32>(PartIndex));
                Section.Vertices.reserve(static_cast<SIZE_T>(Geometry.numVertices));
                for (int32 VertexIndex = 0; VertexIndex < Geometry.numVertices; ++VertexIndex)
                {
                    FSKVertex Vertex{};
                    Vertex.Position = FSKVector3f(
                        Geometry.vertex[VertexIndex * 3 + 0],
                        Geometry.vertex[VertexIndex * 3 + 1],
                        Geometry.vertex[VertexIndex * 3 + 2]);

                    if (Geometry.normal)
                    {
                        Vertex.Normal = FSKVector3f(
                            Geometry.normal[VertexIndex * 3 + 0],
                            Geometry.normal[VertexIndex * 3 + 1],
                            Geometry.normal[VertexIndex * 3 + 2]);
                    }
                    else
                    {
                        Vertex.Normal = FSKVector3f(0.0f, 0.0f, 1.0f);
                    }
                    Section.Vertices.push_back(Vertex);
                }

                Section.Indices.reserve(static_cast<SIZE_T>(Part.indexSolid));
                for (int32 LocalIndex = 0; LocalIndex < Part.indexSolid; ++LocalIndex)
                {
                    Section.Indices.push_back(Geometry.indexSolid[SolidIndexOffset + LocalIndex]);
                }
                SolidIndexOffset += Part.indexSolid;
                Mesh.Sections.push_back(std::move(Section));
            }

            const uint64 GeometryKey = OutDocument.GeometryRegistry.RegisterMesh(
                "CSFGeometry_" + std::to_string(GeometryIndex),
                Mesh,
                FSKGeometryBuildSettings{}
            );
            GeometryKeys[static_cast<SIZE_T>(GeometryIndex)] = GeometryKey;
            OutDocument.GeometryKeys.push_back(GeometryKey);
            if (const FSKGeometryData* pGeometryData = OutDocument.GeometryRegistry.Find(GeometryKey))
            {
                GeometryBounds[static_cast<SIZE_T>(GeometryIndex)] = pGeometryData->Bounds;
            }
        }

        uint32 StableIdCounter = 1;
        for (int32 NodeIndex = 0; NodeIndex < pFile->numNodes; ++NodeIndex)
        {
            const CSFNode& Node = pFile->nodes[NodeIndex];
            if (Node.geometryIDX < 0 || Node.geometryIDX >= pFile->numGeometries)
            {
                continue;
            }

            const uint64 GeometryKey = GeometryKeys[static_cast<SIZE_T>(Node.geometryIDX)];
            const FSKGeometryData* pGeometryData = OutDocument.GeometryRegistry.Find(GeometryKey);
            if (!pGeometryData)
            {
                continue;
            }

            const FAppMat4 NodeWorld = LoadMatrixColumnMajor(Node.worldTM);
            const FSKAabb& GeometryAabb = GeometryBounds[static_cast<SIZE_T>(Node.geometryIDX)];
            const FAppVec3 LocalCenter = MakeVec3(
                0.5f * (GeometryAabb.Min.X + GeometryAabb.Max.X),
                0.5f * (GeometryAabb.Min.Y + GeometryAabb.Max.Y),
                0.5f * (GeometryAabb.Min.Z + GeometryAabb.Max.Z));

            for (int32 PartIndex = 0; PartIndex < Node.numParts; ++PartIndex)
            {
                const CSFNodePart& NodePart = Node.parts[PartIndex];
                FSkCadSceneDrawItem DrawItem{};
                DrawItem.GeometryKey = GeometryKey;
                DrawItem.SectionIndex = PartIndex;
                DrawItem.MaterialIndex = std::clamp(NodePart.materialIDX, 0, std::max(0, pFile->numMaterials - 1));
                DrawItem.WorldMatrix = (NodePart.nodeIDX >= 0 && NodePart.nodeIDX < pFile->numNodes)
                    ? LoadMatrixColumnMajor(pFile->nodes[NodePart.nodeIDX].worldTM)
                    : NodeWorld;
                DrawItem.BoundsCenter = TransformPoint(DrawItem.WorldMatrix, LocalCenter);
                DrawItem.StableId = StableIdCounter++;
                OutDocument.DrawItems.push_back(DrawItem);
                ExpandBounds(OutDocument.SceneMin, OutDocument.SceneMax, DrawItem.BoundsCenter);
            }
        }

        OutDocument.SceneCenter = MakeVec3(
            0.5f * (OutDocument.SceneMin.X + OutDocument.SceneMax.X),
            0.5f * (OutDocument.SceneMin.Y + OutDocument.SceneMax.Y),
            0.5f * (OutDocument.SceneMin.Z + OutDocument.SceneMax.Z));

        const FAppVec3 Extents = AppVec3Sub(OutDocument.SceneMax, OutDocument.SceneMin);
        OutDocument.SceneRadius = std::max(1.0f, 0.5f * std::max({ Extents.X, Extents.Y, Extents.Z }));

        for (FSkCadSceneDrawItem& DrawItem : OutDocument.DrawItems)
        {
            DrawItem.ExplosionDirection = AppVec3Normalize(AppVec3Sub(DrawItem.BoundsCenter, OutDocument.SceneCenter));
        }

        CSFileMemory_delete(pMemory);
        return true;
    }
}
