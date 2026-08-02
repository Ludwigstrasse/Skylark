#include "Render/RenderCore.h"

#include <algorithm>
#include <utility>

namespace skylark::cam
{
namespace
{
std::uint64_t EstimateChunkVertexBytes(std::uint32_t triangleCount) noexcept
{
    return static_cast<std::uint64_t>(triangleCount) * 3ULL * 6ULL * sizeof(float);
}
}

void GpuUploadQueue::Clear()
{
    m_requests.clear();
    m_nextRequestId = 1ULL;
}

void GpuUploadQueue::EnqueueLargeMeshProxy(const LargeMeshRenderProxy& proxy)
{
    for (const LargeMeshRenderChunkProxy& chunk : proxy.chunks)
    {
        GpuBufferUploadRequest request;
        request.requestId = m_nextRequestId++;
        request.assetName = proxy.name;
        request.chunkId = chunk.chunkId;
        request.firstTriangle = chunk.firstTriangle;
        request.triangleCount = chunk.triangleCount;
        request.estimatedVertexBytes = chunk.estimatedVertexBytes;
        request.bounds = chunk.bounds;
        request.state = GpuUploadRequestState::Pending;
        m_requests.push_back(std::move(request));
    }
}

std::uint32_t GpuUploadQueue::PumpUploads(std::uint32_t maxRequestsToUpload)
{
    std::uint32_t uploaded = 0U;
    for (GpuBufferUploadRequest& request : m_requests)
    {
        if (uploaded >= maxRequestsToUpload) { break; }
        if (request.state == GpuUploadRequestState::Pending)
        {
            request.state = GpuUploadRequestState::Uploaded;
            ++uploaded;
        }
    }
    return uploaded;
}

const std::deque<GpuBufferUploadRequest>& GpuUploadQueue::GetRequests() const noexcept { return m_requests; }

GpuUploadQueueSnapshot GpuUploadQueue::CreateSnapshot() const
{
    GpuUploadQueueSnapshot snapshot;
    snapshot.totalRequests = static_cast<std::uint64_t>(m_requests.size());
    for (const GpuBufferUploadRequest& request : m_requests)
    {
        if (request.state == GpuUploadRequestState::Pending)
        {
            ++snapshot.pendingRequests;
            snapshot.pendingBytes += request.estimatedVertexBytes;
        }
        else if (request.state == GpuUploadRequestState::Uploaded)
        {
            ++snapshot.uploadedRequests;
            snapshot.uploadedBytes += request.estimatedVertexBytes;
        }
        else
        {
            ++snapshot.failedRequests;
        }
    }
    return snapshot;
}

std::vector<RenderProxyDescriptor> RenderProxyBuilder::BuildM0ProxyList(const CamProject& project) const
{
    std::vector<RenderProxyDescriptor> proxies;
    for (const ImportedModel& model : project.GetImportedModels())
    {
        proxies.push_back({model.geometryKind == CamGeometryKind::BRep ? RenderProxyKind::BRep : RenderProxyKind::LargeMesh, model.name, true});
    }
    for (const CamSetup& setup : project.GetSetups())
    {
        proxies.push_back({RenderProxyKind::Stock, setup.stock.name, true});
        proxies.push_back({RenderProxyKind::Fixture, setup.fixture.name, true});
        proxies.push_back({RenderProxyKind::Wcs, setup.wcs.name, true});
    }
    for (const ToolpathDocument& toolpath : project.GetToolpathDocuments())
    {
        proxies.push_back({RenderProxyKind::Toolpath, toolpath.name, toolpath.visible});
    }
    return proxies;
}

LargeMeshRenderProxy RenderProxyBuilder::BuildLargeMeshRenderProxy(const LargeMeshAsset& asset) const
{
    LargeMeshRenderProxy proxy;
    proxy.name = asset.GetName();
    proxy.sourcePath = asset.GetSourcePath();
    proxy.triangleCount = asset.GetTriangleCount();
    proxy.bounds = asset.GetBounds();
    proxy.bvhReady = asset.IsBvhReady();
    proxy.lodReady = asset.IsLodReady();
    proxy.bvhNodeCount = asset.GetBvhNodeCount();
    proxy.lodLevelCount = asset.GetLodLevelCount();
    proxy.chunks.reserve(asset.GetChunks().size());
    for (const MeshChunkDescriptor& chunk : asset.GetChunks())
    {
        LargeMeshRenderChunkProxy chunkProxy;
        chunkProxy.chunkId = chunk.chunkId;
        chunkProxy.firstTriangle = chunk.firstTriangle;
        chunkProxy.triangleCount = chunk.triangleCount;
        chunkProxy.bounds = chunk.bounds;
        chunkProxy.estimatedVertexBytes = EstimateChunkVertexBytes(chunk.triangleCount);
        chunkProxy.queuedForUpload = true;
        chunkProxy.uploaded = false;
        proxy.chunks.push_back(std::move(chunkProxy));
    }
    return proxy;
}

std::vector<LargeMeshRenderProxy> RenderProxyBuilder::BuildLargeMeshRenderProxies(const std::vector<LargeMeshAsset>& assets) const
{
    std::vector<LargeMeshRenderProxy> proxies;
    proxies.reserve(assets.size());
    for (const LargeMeshAsset& asset : assets)
    {
        proxies.push_back(BuildLargeMeshRenderProxy(asset));
    }
    return proxies;
}

SceneAdapterSnapshot SkylarkSceneAdapter::CreateSnapshot(const CamProject& project) const
{
    SceneAdapterSnapshot snapshot;
    snapshot.proxies = m_proxyBuilder.BuildM0ProxyList(project);
    return snapshot;
}

SceneAdapterSnapshot SkylarkSceneAdapter::CreateSnapshot(const CamProject& project, const std::vector<LargeMeshAsset>& largeMeshAssets, const GpuUploadQueue& uploadQueue) const
{
    SceneAdapterSnapshot snapshot;
    snapshot.proxies = m_proxyBuilder.BuildM0ProxyList(project);
    snapshot.largeMeshProxies = m_proxyBuilder.BuildLargeMeshRenderProxies(largeMeshAssets);
    snapshot.uploadQueue = uploadQueue.CreateSnapshot();
    return snapshot;
}

std::string ToString(RenderProxyKind kind)
{
    switch (kind)
    {
    case RenderProxyKind::LargeMesh: return "LargeMeshRenderProxy";
    case RenderProxyKind::BRep: return "BRepRenderProxy";
    case RenderProxyKind::Stock: return "StockRenderProxy";
    case RenderProxyKind::Fixture: return "FixtureRenderProxy";
    case RenderProxyKind::Toolpath: return "ToolpathRenderProxy";
    case RenderProxyKind::Selection: return "SelectionRenderProxy";
    case RenderProxyKind::Wcs: return "WCSRenderProxy";
    default: return "UnknownRenderProxy";
    }
}

std::string ToString(GpuUploadRequestState state)
{
    switch (state)
    {
    case GpuUploadRequestState::Pending: return "Pending";
    case GpuUploadRequestState::Uploaded: return "Uploaded";
    case GpuUploadRequestState::Failed: return "Failed";
    default: return "Unknown";
    }
}
}
