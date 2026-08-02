#pragma once

#include "LargeMesh/LargeMesh.h"
#include "Model/CamProject.h"

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace skylark::cam
{
enum class RenderProxyKind { LargeMesh, BRep, Stock, Fixture, Toolpath, Selection, Wcs };

enum class GpuUploadRequestState
{
    Pending,
    Uploaded,
    Failed
};

struct RenderProxyDescriptor
{
    RenderProxyKind kind{RenderProxyKind::LargeMesh};
    std::string name;
    bool visible{true};
};

struct LargeMeshRenderChunkProxy
{
    std::uint32_t chunkId{0};
    std::uint64_t firstTriangle{0};
    std::uint32_t triangleCount{0};
    BoundingBox3d bounds;
    std::uint64_t estimatedVertexBytes{0};
    bool queuedForUpload{false};
    bool uploaded{false};
};

struct LargeMeshRenderProxy
{
    std::string name;
    std::string sourcePath;
    std::uint64_t triangleCount{0};
    BoundingBox3d bounds;
    bool visible{true};
    bool bvhReady{false};
    bool lodReady{false};
    std::uint32_t bvhNodeCount{0};
    std::uint32_t lodLevelCount{0};
    std::vector<LargeMeshRenderChunkProxy> chunks;
};

struct GpuBufferUploadRequest
{
    std::uint64_t requestId{0};
    std::string assetName;
    std::uint32_t chunkId{0};
    std::uint64_t firstTriangle{0};
    std::uint32_t triangleCount{0};
    std::uint64_t estimatedVertexBytes{0};
    BoundingBox3d bounds;
    GpuUploadRequestState state{GpuUploadRequestState::Pending};
};

struct GpuUploadQueueSnapshot
{
    std::uint64_t totalRequests{0};
    std::uint64_t pendingRequests{0};
    std::uint64_t uploadedRequests{0};
    std::uint64_t failedRequests{0};
    std::uint64_t pendingBytes{0};
    std::uint64_t uploadedBytes{0};
};

class GpuUploadQueue
{
public:
    void Clear();
    void EnqueueLargeMeshProxy(const LargeMeshRenderProxy& proxy);
    std::uint32_t PumpUploads(std::uint32_t maxRequestsToUpload);
    const std::deque<GpuBufferUploadRequest>& GetRequests() const noexcept;
    GpuUploadQueueSnapshot CreateSnapshot() const;

private:
    std::uint64_t m_nextRequestId{1};
    std::deque<GpuBufferUploadRequest> m_requests;
};

class RenderProxyBuilder
{
public:
    std::vector<RenderProxyDescriptor> BuildM0ProxyList(const CamProject& project) const;
    LargeMeshRenderProxy BuildLargeMeshRenderProxy(const LargeMeshAsset& asset) const;
    std::vector<LargeMeshRenderProxy> BuildLargeMeshRenderProxies(const std::vector<LargeMeshAsset>& assets) const;
};

struct SceneAdapterSnapshot
{
    std::vector<RenderProxyDescriptor> proxies;
    std::vector<LargeMeshRenderProxy> largeMeshProxies;
    GpuUploadQueueSnapshot uploadQueue;
    bool showStock{true};
    bool showToolpath{true};
    bool showWcs{true};
    bool showDiagnostics{true};
};

class SkylarkSceneAdapter
{
public:
    SceneAdapterSnapshot CreateSnapshot(const CamProject& project) const;
    SceneAdapterSnapshot CreateSnapshot(const CamProject& project, const std::vector<LargeMeshAsset>& largeMeshAssets, const GpuUploadQueue& uploadQueue) const;
private:
    RenderProxyBuilder m_proxyBuilder;
};

std::string ToString(RenderProxyKind kind);
std::string ToString(GpuUploadRequestState state);
}
