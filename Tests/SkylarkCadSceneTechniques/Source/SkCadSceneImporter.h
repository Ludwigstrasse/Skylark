#pragma once

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKCore/SKString.h"

namespace Skylark::CadSceneTest
{
    struct FAppVec3
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
    };

    struct FAppVec4
    {
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        float W = 1.0f;
    };

    struct FAppMat4
    {
        float M[16]{}; // column-major, OpenGL-friendly
    };

    struct FSkCadSceneMaterial
    {
        FSKString Name;
        FAppVec4 BaseColor{};
    };

    struct FSkCadSceneDrawItem
    {
        uint64 GeometryKey = 0;
        int32 SectionIndex = 0;
        int32 MaterialIndex = 0;
        FAppMat4 WorldMatrix{};
        FAppVec3 BoundsCenter{};
        FAppVec3 ExplosionDirection{};
        uint32 StableId = 0;
    };

    struct FSkCadSceneDocument
    {
        FSKGeometryRegistry GeometryRegistry;
        TArray<FSkCadSceneMaterial> Materials;
        TArray<FSkCadSceneDrawItem> DrawItems;
        TArray<uint64> GeometryKeys;
        FAppVec3 SceneMin{};
        FAppVec3 SceneMax{};
        FAppVec3 SceneCenter{};
        float SceneRadius = 1.0f;
    };

    bool LoadCadSceneDocument(const char* pFilename, FSkCadSceneDocument& OutDocument, FSKString& OutError);

    FAppMat4 AppMat4Identity();
    FAppMat4 AppMat4Translation(float X, float Y, float Z);
    FAppMat4 AppMat4Multiply(const FAppMat4& A, const FAppMat4& B);
    FAppMat4 AppMat4Perspective(float fovyRadians, float aspect, float zNear, float zFar);
    FAppMat4 AppMat4LookAt(const FAppVec3& Eye, const FAppVec3& Center, const FAppVec3& Up);
    FAppVec3 AppVec3Add(const FAppVec3& A, const FAppVec3& B);
    FAppVec3 AppVec3Sub(const FAppVec3& A, const FAppVec3& B);
    FAppVec3 AppVec3Scale(const FAppVec3& V, float S);
    float AppVec3Length(const FAppVec3& V);
    FAppVec3 AppVec3Normalize(const FAppVec3& V);
}
