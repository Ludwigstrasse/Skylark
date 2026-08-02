#pragma once
#include <memory>

#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
#include "SKRHI/SKRHI.h"
#include "SKRenderer/SKGeometryResidencyCache.h"

namespace Skylark
{
    struct FSKResidentMeshSectionBuffers
    {
        FSKObjectId OwnerId{};
        uint32 SectionHitProxyId = 0;
        FSKVector3f BaseColor = FSKVector3f(0.75f, 0.77f, 0.80f);
        std::unique_ptr<ISKRHIResidentTriangleBuffer> Buffer;
    };

    struct FSKResidentMeshGeometryBuffers
    {
        uint64 GeometryKey = 0;
        FSKString DebugName;
        TArray<FSKResidentMeshSectionBuffers> Sections;
        bool bResidentSupported = false;
    };

    struct FSKResidentEdgeSectionBuffers
    {
        FSKObjectId OwnerId{};
        uint32 SectionHitProxyId = 0;
        std::unique_ptr<ISKRHIResidentLineBuffer> All;
        std::unique_ptr<ISKRHIResidentLineBuffer> Boundary;
        std::unique_ptr<ISKRHIResidentLineBuffer> Sharp;
    };

    struct FSKResidentEdgeGeometryBuffers
    {
        uint64 GeometryKey = 0;
        FSKString DebugName;
        TArray<FSKResidentEdgeSectionBuffers> Sections;
        bool bResidentSupported = false;
    };

    struct FSKGpuResidentBufferStats
    {
        SIZE_T MeshGeometryCount = 0;
        SIZE_T EdgeGeometryCount = 0;
        SIZE_T ResidentMeshSectionCount = 0;
        SIZE_T ResidentEdgeSectionCount = 0;
        SIZE_T ResidentTriangleVertexCount = 0;
        SIZE_T ResidentTriangleIndexCount = 0;
        SIZE_T ResidentLineVertexCount = 0;
    };

    class FSKGpuResidentBufferCache final
    {
    public:
        static FSKGpuResidentBufferCache& Get();

        const FSKResidentMeshGeometryBuffers& GetOrCreateMeshBuffers(ISKRHICommandList& Cmd, const FSKGeometryData& Geometry);
        const FSKResidentEdgeGeometryBuffers& GetOrCreateEdgeBuffers(ISKRHICommandList& Cmd, const FSKGeometryData& Geometry);

        void Clear();
        const FSKGpuResidentBufferStats& GetStats() const;

    private:
        void RebuildStats();

    private:
        TMap<uint64, FSKResidentMeshGeometryBuffers> MeshBuffers;
        TMap<uint64, FSKResidentEdgeGeometryBuffers> EdgeBuffers;
        FSKGpuResidentBufferStats Stats{};
    };
}
