#include "LargeMesh/LargeMesh.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <numeric>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
constexpr std::uint64_t kBinaryStlHeaderBytes = 84ULL;
constexpr std::uint64_t kBinaryStlTriangleBytes = 50ULL;
constexpr std::uint64_t kTwentyMillionTriangles = 20000000ULL;

float ReadFloat32LE(const char* data) noexcept
{
    float value = 0.0F;
    static_assert(sizeof(value) == 4U, "Unexpected float size.");
    unsigned char* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(data[0]);
    out[1] = static_cast<unsigned char>(data[1]);
    out[2] = static_cast<unsigned char>(data[2]);
    out[3] = static_cast<unsigned char>(data[3]);
    return value;
}

std::uint32_t ReadUInt32LE(const char* data) noexcept
{
    return static_cast<std::uint32_t>(static_cast<unsigned char>(data[0])) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(data[1])) << 8U) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(data[2])) << 16U) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(data[3])) << 24U);
}

Vec3d ReadVec3fLE(const char* data) noexcept
{
    return {
        static_cast<double>(ReadFloat32LE(data + 0)),
        static_cast<double>(ReadFloat32LE(data + 4)),
        static_cast<double>(ReadFloat32LE(data + 8))};
}

std::uint32_t ComputePreviewStride(std::uint64_t triangleCount, std::uint32_t maxPreviewTriangles) noexcept
{
    if (maxPreviewTriangles == 0U || triangleCount <= static_cast<std::uint64_t>(maxPreviewTriangles)) { return 1U; }
    const std::uint64_t stride = (triangleCount + static_cast<std::uint64_t>(maxPreviewTriangles) - 1ULL) / static_cast<std::uint64_t>(maxPreviewTriangles);
    return static_cast<std::uint32_t>(std::min<std::uint64_t>(stride, std::numeric_limits<std::uint32_t>::max()));
}

std::string FileNameFromPath(const std::filesystem::path& path)
{
    const std::filesystem::path filename = path.filename();
    if (filename.empty()) { return path.string(); }
    return filename.string();
}

std::uint32_t LongestAxis(const BoundingBox3d& box) noexcept
{
    const Vec3d size = box.Size();
    if (size.x >= size.y && size.x >= size.z) { return 0U; }
    if (size.y >= size.x && size.y >= size.z) { return 1U; }
    return 2U;
}

bool LessChunkCenter(const MeshChunkDescriptor& lhs, const MeshChunkDescriptor& rhs, std::uint32_t axis) noexcept
{
    const Vec3d lc = lhs.bounds.Center();
    const Vec3d rc = rhs.bounds.Center();
    if (axis == 0U) { return lc.x < rc.x; }
    if (axis == 1U) { return lc.y < rc.y; }
    return lc.z < rc.z;
}

BoundingBox3d ComputeChunkRangeBounds(
    const std::vector<MeshChunkDescriptor>& chunks,
    const std::vector<std::uint32_t>& chunkIndices,
    std::uint32_t begin,
    std::uint32_t end)
{
    BoundingBox3d bounds;
    for (std::uint32_t i = begin; i < end; ++i)
    {
        bounds.Expand(chunks[chunkIndices[i]].bounds);
    }
    return bounds;
}
}

void LargeMeshAsset::SetName(std::string name) { m_name = std::move(name); }
const std::string& LargeMeshAsset::GetName() const noexcept { return m_name; }
void LargeMeshAsset::SetSourcePath(std::string path) { m_sourcePath = std::move(path); }
const std::string& LargeMeshAsset::GetSourcePath() const noexcept { return m_sourcePath; }
void LargeMeshAsset::SetTriangleCount(std::uint64_t count) noexcept { m_triangleCount = count; }
std::uint64_t LargeMeshAsset::GetTriangleCount() const noexcept { return m_triangleCount; }
void LargeMeshAsset::SetFileSizeBytes(std::uint64_t fileSizeBytes) noexcept { m_fileSizeBytes = fileSizeBytes; }
std::uint64_t LargeMeshAsset::GetFileSizeBytes() const noexcept { return m_fileSizeBytes; }
void LargeMeshAsset::SetImportTimeSeconds(double seconds) noexcept { m_importTimeSeconds = seconds; }
double LargeMeshAsset::GetImportTimeSeconds() const noexcept { return m_importTimeSeconds; }
void LargeMeshAsset::SetEstimatedMemoryBytes(std::uint64_t bytes) noexcept { m_estimatedMemoryBytes = bytes; }
std::uint64_t LargeMeshAsset::GetEstimatedMemoryBytes() const noexcept { return m_estimatedMemoryBytes; }
void LargeMeshAsset::AddChunk(MeshChunkDescriptor chunk) { m_chunks.push_back(std::move(chunk)); }
void LargeMeshAsset::ClearChunks() { m_chunks.clear(); }
const std::vector<MeshChunkDescriptor>& LargeMeshAsset::GetChunks() const noexcept { return m_chunks; }
void LargeMeshAsset::SetBounds(BoundingBox3d bounds) noexcept { m_bounds = bounds; }
const BoundingBox3d& LargeMeshAsset::GetBounds() const noexcept { return m_bounds; }
void LargeMeshAsset::SetPreviewStride(std::uint32_t stride) noexcept { m_previewStride = std::max<std::uint32_t>(stride, 1U); }
std::uint32_t LargeMeshAsset::GetPreviewStride() const noexcept { return m_previewStride; }
void LargeMeshAsset::ReservePreviewTriangles(std::size_t count) { m_previewTriangles.reserve(count); }
void LargeMeshAsset::AddPreviewTriangle(MeshTriangle3d triangle) { m_previewTriangles.push_back(std::move(triangle)); }
const std::vector<MeshTriangle3d>& LargeMeshAsset::GetPreviewTriangles() const noexcept { return m_previewTriangles; }
void LargeMeshAsset::MarkBvhQueued() noexcept { if (m_bvhState != LargeMeshBuildState::Ready) { m_bvhState = LargeMeshBuildState::Queued; } }
void LargeMeshAsset::MarkLodQueued() noexcept { if (m_lodState != LargeMeshBuildState::Ready) { m_lodState = LargeMeshBuildState::Queued; } }
void LargeMeshAsset::MarkBvhRunning() noexcept { if (m_bvhState != LargeMeshBuildState::Ready) { m_bvhState = LargeMeshBuildState::Running; } }
void LargeMeshAsset::MarkLodRunning() noexcept { if (m_lodState != LargeMeshBuildState::Ready) { m_lodState = LargeMeshBuildState::Running; } }
void LargeMeshAsset::MarkBvhReady(bool ready) noexcept { m_bvhState = ready ? LargeMeshBuildState::Ready : LargeMeshBuildState::Failed; }
void LargeMeshAsset::MarkLodReady(bool ready) noexcept { m_lodState = ready ? LargeMeshBuildState::Ready : LargeMeshBuildState::Failed; }
bool LargeMeshAsset::IsBvhReady() const noexcept { return m_bvhState == LargeMeshBuildState::Ready; }
bool LargeMeshAsset::IsLodReady() const noexcept { return m_lodState == LargeMeshBuildState::Ready; }
LargeMeshBuildState LargeMeshAsset::GetBvhState() const noexcept { return m_bvhState; }
LargeMeshBuildState LargeMeshAsset::GetLodState() const noexcept { return m_lodState; }
void LargeMeshAsset::SetBvhNodeCount(std::uint32_t count) noexcept { m_bvhNodeCount = count; }
std::uint32_t LargeMeshAsset::GetBvhNodeCount() const noexcept { return m_bvhNodeCount; }
void LargeMeshAsset::SetLodLevelCount(std::uint32_t count) noexcept { m_lodLevelCount = count; }
std::uint32_t LargeMeshAsset::GetLodLevelCount() const noexcept { return m_lodLevelCount; }
void LargeMeshAsset::ApplyAccelerationBuildResult(const LargeMeshAccelerationBuildResult& result)
{
    SetBvhNodeCount(static_cast<std::uint32_t>(result.bvh.nodes.size()));
    SetLodLevelCount(static_cast<std::uint32_t>(result.lod.levels.size()));
    MarkBvhReady(result.bvh.state == LargeMeshBuildState::Ready);
    MarkLodReady(result.lod.state == LargeMeshBuildState::Ready);
}

MeshChunkBuilder::MeshChunkBuilder(std::uint32_t preferredTrianglesPerChunk)
    : m_preferredTrianglesPerChunk(std::max<std::uint32_t>(preferredTrianglesPerChunk, 1U))
{
}

LargeMeshAsset MeshChunkBuilder::CreateEmptyAsset(std::string name, std::uint64_t triangleCount) const
{
    LargeMeshAsset asset;
    asset.SetName(std::move(name));
    asset.SetTriangleCount(triangleCount);
    const std::uint64_t chunkCount = (triangleCount + static_cast<std::uint64_t>(m_preferredTrianglesPerChunk) - 1ULL) / static_cast<std::uint64_t>(m_preferredTrianglesPerChunk);
    for (std::uint64_t i = 0; i < chunkCount; ++i)
    {
        const std::uint64_t first = i * static_cast<std::uint64_t>(m_preferredTrianglesPerChunk);
        const std::uint64_t remaining = triangleCount - first;
        const std::uint32_t count = static_cast<std::uint32_t>(std::min<std::uint64_t>(remaining, static_cast<std::uint64_t>(m_preferredTrianglesPerChunk)));
        asset.AddChunk({static_cast<std::uint32_t>(i), first, count, {}});
    }
    return asset;
}

std::vector<MeshChunkDescriptor> MeshChunkBuilder::BuildChunkTable(std::uint64_t triangleCount, const std::vector<BoundingBox3d>& chunkBounds) const
{
    std::vector<MeshChunkDescriptor> chunks;
    const std::uint64_t chunkCount = (triangleCount + static_cast<std::uint64_t>(m_preferredTrianglesPerChunk) - 1ULL) / static_cast<std::uint64_t>(m_preferredTrianglesPerChunk);
    chunks.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(chunkCount, static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max()))));
    for (std::uint64_t i = 0; i < chunkCount; ++i)
    {
        const std::uint64_t first = i * static_cast<std::uint64_t>(m_preferredTrianglesPerChunk);
        const std::uint64_t remaining = triangleCount - first;
        const std::uint32_t count = static_cast<std::uint32_t>(std::min<std::uint64_t>(remaining, static_cast<std::uint64_t>(m_preferredTrianglesPerChunk)));
        BoundingBox3d bounds;
        if (i < static_cast<std::uint64_t>(chunkBounds.size())) { bounds = chunkBounds[static_cast<std::size_t>(i)]; }
        chunks.push_back({static_cast<std::uint32_t>(i), first, count, bounds});
    }
    return chunks;
}

MeshBvhBuildResult MeshBvhBuilder::BuildFromChunks(const std::vector<MeshChunkDescriptor>& chunks) const
{
    const auto t0 = std::chrono::steady_clock::now();
    MeshBvhBuildResult result;
    if (chunks.empty())
    {
        result.state = LargeMeshBuildState::Failed;
        result.message = "No chunks available for BVH build.";
        return result;
    }

    std::vector<std::uint32_t> chunkIndices(chunks.size());
    std::iota(chunkIndices.begin(), chunkIndices.end(), 0U);
    result.nodes.reserve(chunks.size() * 2U);
    BuildRecursive(chunks, chunkIndices, 0U, static_cast<std::uint32_t>(chunkIndices.size()), result.nodes);
    result.leafCount = static_cast<std::uint32_t>(std::count_if(result.nodes.begin(), result.nodes.end(), [](const MeshBvhNode& node) { return node.leaf; }));
    result.state = LargeMeshBuildState::Ready;
    result.message = "Chunk-level BVH built from LargeMeshAsset chunk descriptors.";
    const auto t1 = std::chrono::steady_clock::now();
    result.buildTimeSeconds = std::chrono::duration<double>(t1 - t0).count();
    return result;
}

std::uint32_t MeshBvhBuilder::BuildRecursive(
    const std::vector<MeshChunkDescriptor>& chunks,
    std::vector<std::uint32_t>& chunkIndices,
    std::uint32_t begin,
    std::uint32_t end,
    std::vector<MeshBvhNode>& nodes) const
{
    const std::uint32_t nodeIndex = static_cast<std::uint32_t>(nodes.size());
    nodes.push_back({});
    MeshBvhNode& node = nodes.back();
    node.bounds = ComputeChunkRangeBounds(chunks, chunkIndices, begin, end);
    node.firstChunk = begin;
    node.chunkCount = end - begin;
    node.leaf = node.chunkCount <= 4U;

    if (!node.leaf)
    {
        const std::uint32_t axis = LongestAxis(node.bounds);
        const std::uint32_t mid = begin + (end - begin) / 2U;
        std::nth_element(
            chunkIndices.begin() + begin,
            chunkIndices.begin() + mid,
            chunkIndices.begin() + end,
            [&chunks, axis](std::uint32_t lhs, std::uint32_t rhs) { return LessChunkCenter(chunks[lhs], chunks[rhs], axis); });
        node.leftChild = BuildRecursive(chunks, chunkIndices, begin, mid, nodes);
        node.rightChild = BuildRecursive(chunks, chunkIndices, mid, end, nodes);
    }
    return nodeIndex;
}

MeshLodBuildResult MeshLodBuilder::BuildFromAsset(const LargeMeshAsset& asset) const
{
    const auto t0 = std::chrono::steady_clock::now();
    MeshLodBuildResult result;
    if (asset.GetTriangleCount() == 0ULL)
    {
        result.state = LargeMeshBuildState::Failed;
        result.message = "No triangles available for LOD build.";
        return result;
    }

    const std::uint32_t strides[] = {1U, 2U, 4U, 8U, 16U, 32U, 64U};
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(sizeof(strides) / sizeof(strides[0])); ++i)
    {
        const std::uint32_t stride = strides[i];
        const std::uint64_t representative = (asset.GetTriangleCount() + static_cast<std::uint64_t>(stride) - 1ULL) / static_cast<std::uint64_t>(stride);
        if (i > 0U && representative < 8000ULL) { break; }
        MeshLodLevel level;
        level.levelIndex = i;
        level.triangleStride = stride;
        level.representativeTriangleCount = representative;
        level.chunkCount = static_cast<std::uint32_t>(asset.GetChunks().size());
        result.levels.push_back(level);
    }

    result.state = LargeMeshBuildState::Ready;
    result.message = "LOD descriptors built without expanding per-triangle objects.";
    const auto t1 = std::chrono::steady_clock::now();
    result.buildTimeSeconds = std::chrono::duration<double>(t1 - t0).count();
    return result;
}

std::future<LargeMeshAccelerationBuildResult> LargeMeshAsyncBuilder::StartBackgroundBuild(const LargeMeshAsset& asset) const
{
    return std::async(std::launch::async, [snapshot = asset, builder = *this]() {
        return builder.BuildNow(snapshot);
    });
}

LargeMeshAccelerationBuildResult LargeMeshAsyncBuilder::BuildNow(const LargeMeshAsset& asset) const
{
    const auto t0 = std::chrono::steady_clock::now();
    LargeMeshAccelerationBuildResult result;
    result.assetName = asset.GetName();
    result.triangleCount = asset.GetTriangleCount();
    result.chunkCount = static_cast<std::uint32_t>(asset.GetChunks().size());
    MeshBvhBuilder bvhBuilder;
    MeshLodBuilder lodBuilder;
    result.bvh = bvhBuilder.BuildFromChunks(asset.GetChunks());
    result.lod = lodBuilder.BuildFromAsset(asset);
    const auto t1 = std::chrono::steady_clock::now();
    result.totalBuildTimeSeconds = std::chrono::duration<double>(t1 - t0).count();
    return result;
}

BinaryStlReadResult BinaryStlFastReader::ReadBinaryStl(const std::filesystem::path& path, const LargeMeshImportOptions& options) const
{
    BinaryStlReadResult result;
    result.diagnostics.format = "Binary STL";
    result.asset.SetName(FileNameFromPath(path));
    result.asset.SetSourcePath(path.string());

    const auto t0 = std::chrono::steady_clock::now();
    std::error_code ec;
    const std::uint64_t fileSize = static_cast<std::uint64_t>(std::filesystem::file_size(path, ec));
    if (ec || fileSize < kBinaryStlHeaderBytes)
    {
        result.diagnostics.AddWarning("Binary STL file does not exist or is smaller than the STL header.");
        return result;
    }
    result.asset.SetFileSizeBytes(fileSize);

    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        result.diagnostics.AddWarning("Failed to open Binary STL file.");
        return result;
    }

    std::array<char, 80> header{};
    input.read(header.data(), static_cast<std::streamsize>(header.size()));
    std::array<char, 4> countBytes{};
    input.read(countBytes.data(), static_cast<std::streamsize>(countBytes.size()));
    if (!input)
    {
        result.diagnostics.AddWarning("Failed to read Binary STL triangle count.");
        return result;
    }

    const std::uint32_t triangleCount = ReadUInt32LE(countBytes.data());
    const std::uint64_t expectedSize = kBinaryStlHeaderBytes + static_cast<std::uint64_t>(triangleCount) * kBinaryStlTriangleBytes;
    if (expectedSize > fileSize)
    {
        result.diagnostics.AddWarning("Binary STL file is truncated: header triangle count exceeds actual file size.");
        return result;
    }
    if (expectedSize < fileSize)
    {
        result.diagnostics.AddWarning("Binary STL file has trailing bytes after triangle payload; import will ignore them.");
    }

    result.asset.SetTriangleCount(triangleCount);
    result.asset.SetEstimatedMemoryBytes(static_cast<std::uint64_t>(triangleCount) * 3ULL * 6ULL * sizeof(float));
    const std::uint32_t trianglesPerChunk = std::max<std::uint32_t>(options.preferredTrianglesPerChunk, 1U);
    const std::uint32_t previewStride = ComputePreviewStride(triangleCount, options.maxPreviewTriangles);
    result.asset.SetPreviewStride(previewStride);

    const std::uint64_t chunkCount = (static_cast<std::uint64_t>(triangleCount) + static_cast<std::uint64_t>(trianglesPerChunk) - 1ULL) / static_cast<std::uint64_t>(trianglesPerChunk);
    std::vector<BoundingBox3d> chunkBounds(static_cast<std::size_t>(chunkCount));
    BoundingBox3d globalBounds;
    std::array<char, static_cast<std::size_t>(kBinaryStlTriangleBytes)> record{};

    if (options.buildPreviewTriangles && options.maxPreviewTriangles > 0U)
    {
        const std::uint64_t reserveCount = (static_cast<std::uint64_t>(triangleCount) + static_cast<std::uint64_t>(previewStride) - 1ULL) / static_cast<std::uint64_t>(previewStride);
        result.asset.ReservePreviewTriangles(static_cast<std::size_t>(std::min<std::uint64_t>(reserveCount, options.maxPreviewTriangles + 1ULL)));
    }

    for (std::uint32_t triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex)
    {
        input.read(record.data(), static_cast<std::streamsize>(record.size()));
        if (!input)
        {
            result.diagnostics.AddWarning("Failed while streaming Binary STL triangle payload.");
            return result;
        }

        MeshTriangle3d triangle;
        triangle.normal = ReadVec3fLE(record.data() + 0);
        triangle.p0 = ReadVec3fLE(record.data() + 12);
        triangle.p1 = ReadVec3fLE(record.data() + 24);
        triangle.p2 = ReadVec3fLE(record.data() + 36);
        if (Length(triangle.normal) <= 1.0e-10)
        {
            triangle.normal = Normalize(Cross(triangle.p1 - triangle.p0, triangle.p2 - triangle.p0));
        }
        else
        {
            triangle.normal = Normalize(triangle.normal);
        }

        globalBounds.Expand(triangle.p0);
        globalBounds.Expand(triangle.p1);
        globalBounds.Expand(triangle.p2);

        const std::size_t chunkIndex = static_cast<std::size_t>(triangleIndex / trianglesPerChunk);
        chunkBounds[chunkIndex].Expand(triangle.p0);
        chunkBounds[chunkIndex].Expand(triangle.p1);
        chunkBounds[chunkIndex].Expand(triangle.p2);

        if (options.buildPreviewTriangles && options.maxPreviewTriangles > 0U && (triangleIndex % previewStride) == 0U)
        {
            result.asset.AddPreviewTriangle(std::move(triangle));
        }
    }

    MeshChunkBuilder chunkBuilder(trianglesPerChunk);
    for (MeshChunkDescriptor chunk : chunkBuilder.BuildChunkTable(triangleCount, chunkBounds))
    {
        result.asset.AddChunk(std::move(chunk));
    }
    result.asset.SetBounds(globalBounds);
    result.asset.MarkBvhQueued();
    result.asset.MarkLodQueued();

    const auto t1 = std::chrono::steady_clock::now();
    const double seconds = std::chrono::duration<double>(t1 - t0).count();
    result.asset.SetImportTimeSeconds(seconds);
    result.diagnostics.success = true;
    result.diagnostics.importedTriangleCount = triangleCount;
    result.diagnostics.importTimeSeconds = seconds;
    result.diagnostics.estimatedMemoryBytes = result.asset.GetEstimatedMemoryBytes();
    result.diagnostics.AddInfo("Binary STL streamed into LargeMeshAsset without BRep conversion.");
    result.diagnostics.AddInfo("Chunked Mesh Asset was created; no per-triangle CAM object allocation was performed.");
    result.diagnostics.AddInfo("BVH and LOD background jobs are queued by ProjectSession after import.");
    return result;
}

BinaryStlReadResult BinaryStlFastReader::CreateM0StubResult(const std::filesystem::path& path) const
{
    MeshChunkBuilder builder;
    BinaryStlReadResult result;
    result.diagnostics.success = true;
    result.diagnostics.format = "Binary STL";
    result.diagnostics.importedTriangleCount = kTwentyMillionTriangles;
    result.diagnostics.estimatedMemoryBytes = 780ULL * 1024ULL * 1024ULL;
    result.asset = builder.CreateEmptyAsset(FileNameFromPath(path), result.diagnostics.importedTriangleCount);
    result.asset.SetSourcePath(path.string());
    result.asset.SetEstimatedMemoryBytes(result.diagnostics.estimatedMemoryBytes);
    result.asset.MarkBvhReady(false);
    result.asset.MarkLodReady(false);
    result.diagnostics.AddInfo("M0 placeholder only; use ReadBinaryStl for real import or CreateSynthetic20MPressureAsset for M2 pressure plumbing.");
    return result;
}

BinaryStlReadResult BinaryStlFastReader::CreateSynthetic20MPressureAsset(const std::string& name, std::uint32_t preferredTrianglesPerChunk) const
{
    const auto t0 = std::chrono::steady_clock::now();
    BinaryStlReadResult result;
    result.diagnostics.success = true;
    result.diagnostics.format = "Synthetic Binary STL pressure descriptor";
    result.diagnostics.importedTriangleCount = kTwentyMillionTriangles;
    result.diagnostics.estimatedMemoryBytes = kTwentyMillionTriangles * 3ULL * 6ULL * sizeof(float);
    result.asset.SetName(name);
    result.asset.SetSourcePath("synthetic://20m-binary-stl-pressure");
    result.asset.SetTriangleCount(kTwentyMillionTriangles);
    result.asset.SetFileSizeBytes(kBinaryStlHeaderBytes + kTwentyMillionTriangles * kBinaryStlTriangleBytes);
    result.asset.SetEstimatedMemoryBytes(result.diagnostics.estimatedMemoryBytes);

    const std::uint32_t trianglesPerChunk = std::max<std::uint32_t>(preferredTrianglesPerChunk, 1U);
    const std::uint64_t chunkCount = (kTwentyMillionTriangles + static_cast<std::uint64_t>(trianglesPerChunk) - 1ULL) / static_cast<std::uint64_t>(trianglesPerChunk);
    std::vector<BoundingBox3d> chunkBounds;
    chunkBounds.reserve(static_cast<std::size_t>(chunkCount));
    BoundingBox3d globalBounds;
    for (std::uint64_t i = 0; i < chunkCount; ++i)
    {
        const double x0 = static_cast<double>(i % 10ULL) * 24.0;
        const double y0 = static_cast<double>((i / 10ULL) % 8ULL) * 18.0;
        const double z0 = static_cast<double>(i / 80ULL) * 4.0;
        BoundingBox3d box;
        box.Expand(Vec3d{x0, y0, z0});
        box.Expand(Vec3d{x0 + 22.0, y0 + 16.0, z0 + 3.5});
        chunkBounds.push_back(box);
        globalBounds.Expand(box);
    }

    MeshChunkBuilder chunkBuilder(trianglesPerChunk);
    for (MeshChunkDescriptor chunk : chunkBuilder.BuildChunkTable(kTwentyMillionTriangles, chunkBounds))
    {
        result.asset.AddChunk(std::move(chunk));
    }
    result.asset.SetBounds(globalBounds);
    result.asset.SetPreviewStride(64U);
    result.asset.MarkBvhQueued();
    result.asset.MarkLodQueued();
    const auto t1 = std::chrono::steady_clock::now();
    const double seconds = std::chrono::duration<double>(t1 - t0).count();
    result.asset.SetImportTimeSeconds(seconds);
    result.diagnostics.importTimeSeconds = seconds;
    result.diagnostics.AddInfo("Synthetic 20M STL pressure entry created chunk table without allocating 20M triangle objects.");
    result.diagnostics.AddInfo("Use --m2-pressure20m <real-file.stl> to stream a real Binary STL pressure file.");
    return result;
}

LargeMeshDiagnostics InspectLargeMeshAsset(const LargeMeshAsset& asset)
{
    return {
        asset.GetName(),
        asset.GetTriangleCount(),
        static_cast<std::uint32_t>(asset.GetChunks().size()),
        asset.GetFileSizeBytes(),
        asset.GetEstimatedMemoryBytes(),
        asset.GetImportTimeSeconds(),
        asset.GetBounds(),
        static_cast<std::uint32_t>(asset.GetPreviewTriangles().size()),
        asset.GetPreviewStride(),
        asset.IsBvhReady(),
        asset.IsLodReady(),
        asset.GetBvhState(),
        asset.GetLodState(),
        asset.GetBvhNodeCount(),
        asset.GetLodLevelCount()};
}

std::string FormatByteSize(std::uint64_t bytes)
{
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    double value = static_cast<double>(bytes);
    std::size_t suffix = 0;
    while (value >= 1024.0 && suffix + 1U < (sizeof(suffixes) / sizeof(suffixes[0])))
    {
        value /= 1024.0;
        ++suffix;
    }
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(suffix == 0U ? 0 : 2) << value << ' ' << suffixes[suffix];
    return stream.str();
}

std::string ToString(LargeMeshBuildState state)
{
    switch (state)
    {
    case LargeMeshBuildState::NotStarted: return "NotStarted";
    case LargeMeshBuildState::Queued: return "Queued";
    case LargeMeshBuildState::Running: return "Running";
    case LargeMeshBuildState::Ready: return "Ready";
    case LargeMeshBuildState::Failed: return "Failed";
    default: return "Unknown";
    }
}
}
