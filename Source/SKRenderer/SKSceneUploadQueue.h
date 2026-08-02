#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
    class ISKRHICommandList;

    enum ESKSceneUploadFlags : uint32
    {
        SK_SceneUpload_None  = 0u,
        SK_SceneUpload_Mesh  = 1u << 0u,
        SK_SceneUpload_Edge  = 1u << 1u,
    };

    inline uint32 operator|(ESKSceneUploadFlags A, ESKSceneUploadFlags B)
    {
        return static_cast<uint32>(A) | static_cast<uint32>(B);
    }

    struct FSKSceneUploadStats
    {
        SIZE_T QueuedGeometryCount = 0;
        SIZE_T ResolveCount = 0;
        SIZE_T MeshRequestCount = 0;
        SIZE_T EdgeRequestCount = 0;
        SIZE_T MeshResidentReadyCount = 0;
        SIZE_T EdgeResidentReadyCount = 0;
    };

    class FSKSceneUploadQueue final
    {
    public:
        static FSKSceneUploadQueue& Get();

        void Reset();
        void QueueVisibleScene(const FSKSceneView& SceneView, uint32 Flags);
        void Resolve(ISKRHICommandList& Cmd, const FSKSceneView& SceneView);

        const FSKSceneUploadStats& GetStats() const;

    private:
        TMap<uint64, uint32> Requests;
        FSKSceneUploadStats Stats{};
    };
}
