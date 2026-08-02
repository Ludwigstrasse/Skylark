#pragma once

#include "Common/CamCore.h"
#include "DataExchange/DataExchange.h"

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <future>
#include <string>
#include <vector>

namespace skylark::cam
{
struct MeshTriangle3d
{
    Vec3d normal{};
    Vec3d p0{};
    Vec3d p1{};
    Vec3d p2{};
};

struct LargeMeshImportOptions
{
    std::uint32_t preferredTrianglesPerChunk{250000U};
    std::uint32_t maxPreviewTriangles{300000U};
    bool buildPreviewTriangles{true};
};

enum class LargeMeshBuildState
{
    NotStarted,
    Queued,
    Running,
    Ready,
    Failed
};

struct MeshChunkDescriptor
{
    std::uint32_t chunkId{0};
    std::uint64_t firstTriangle{0};
    std::uint32_t triangleCount{0};
    BoundingBox3d bounds;
};

struct MeshBvhNode
{
    BoundingBox3d bounds;
    std::uint32_t leftChild{0};
    std::uint32_t rightChild{0};
    std::uint32_t firstChunk{0};
    std::uint32_t chunkCount{0};
    bool leaf{true};
};

struct MeshBvhBuildResult
{
    LargeMeshBuildState state{LargeMeshBuildState::NotStarted};
    std::vector<MeshBvhNode> nodes;
    std::uint32_t leafCount{0};
    double buildTimeSeconds{0.0};
    std::string message;
};

struct MeshLodLevel
{
    std::uint32_t levelIndex{0};
    std::uint32_t triangleStride{1};
    std::uint64_t representativeTriangleCount{0};
    std::uint32_t chunkCount{0};
};

struct MeshLodBuildResult
{
    LargeMeshBuildState state{LargeMeshBuildState::NotStarted};
    std::vector<MeshLodLevel> levels;
    double buildTimeSeconds{0.0};
    std::string message;
};

struct LargeMeshAccelerationBuildResult
{
    std::string assetName;
    std::uint64_t triangleCount{0};
    std::uint32_t chunkCount{0};
    MeshBvhBuildResult bvh;
    MeshLodBuildResult lod;
    double totalBuildTimeSeconds{0.0};
};

class LargeMeshAsset
{
public:
    void SetName(std::string name);
    const std::string& GetName() const noexcept;

    void SetSourcePath(std::string path);
    const std::string& GetSourcePath() const noexcept;

    void SetTriangleCount(std::uint64_t count) noexcept;
    std::uint64_t GetTriangleCount() const noexcept;

    void SetFileSizeBytes(std::uint64_t fileSizeBytes) noexcept;
    std::uint64_t GetFileSizeBytes() const noexcept;

    void SetImportTimeSeconds(double seconds) noexcept;
    double GetImportTimeSeconds() const noexcept;

    void SetEstimatedMemoryBytes(std::uint64_t bytes) noexcept;
    std::uint64_t GetEstimatedMemoryBytes() const noexcept;

    void AddChunk(MeshChunkDescriptor chunk);
    void ClearChunks();
    const std::vector<MeshChunkDescriptor>& GetChunks() const noexcept;

    void SetBounds(BoundingBox3d bounds) noexcept;
    const BoundingBox3d& GetBounds() const noexcept;

    void SetPreviewStride(std::uint32_t stride) noexcept;
    std::uint32_t GetPreviewStride() const noexcept;

    void ReservePreviewTriangles(std::size_t count);
    void AddPreviewTriangle(MeshTriangle3d triangle);
    const std::vector<MeshTriangle3d>& GetPreviewTriangles() const noexcept;

    void MarkBvhQueued() noexcept;
    void MarkLodQueued() noexcept;
    void MarkBvhRunning() noexcept;
    void MarkLodRunning() noexcept;
    void MarkBvhReady(bool ready) noexcept;
    void MarkLodReady(bool ready) noexcept;
    bool IsBvhReady() const noexcept;
    bool IsLodReady() const noexcept;
    LargeMeshBuildState GetBvhState() const noexcept;
    LargeMeshBuildState GetLodState() const noexcept;

    void SetBvhNodeCount(std::uint32_t count) noexcept;
    std::uint32_t GetBvhNodeCount() const noexcept;

    void SetLodLevelCount(std::uint32_t count) noexcept;
    std::uint32_t GetLodLevelCount() const noexcept;

    void ApplyAccelerationBuildResult(const LargeMeshAccelerationBuildResult& result);

private:
    std::string m_name;
    std::string m_sourcePath;
    std::uint64_t m_triangleCount{0};
    std::uint64_t m_fileSizeBytes{0};
    double m_importTimeSeconds{0.0};
    std::uint64_t m_estimatedMemoryBytes{0};
    std::vector<MeshChunkDescriptor> m_chunks;
    BoundingBox3d m_bounds;
    std::uint32_t m_previewStride{1};
    std::vector<MeshTriangle3d> m_previewTriangles;
    LargeMeshBuildState m_bvhState{LargeMeshBuildState::NotStarted};
    LargeMeshBuildState m_lodState{LargeMeshBuildState::NotStarted};
    std::uint32_t m_bvhNodeCount{0};
    std::uint32_t m_lodLevelCount{0};
};

class MeshChunkBuilder
{
public:
    explicit MeshChunkBuilder(std::uint32_t preferredTrianglesPerChunk = 250000U);
    LargeMeshAsset CreateEmptyAsset(std::string name, std::uint64_t triangleCount) const;
    std::vector<MeshChunkDescriptor> BuildChunkTable(std::uint64_t triangleCount, const std::vector<BoundingBox3d>& chunkBounds) const;
private:
    std::uint32_t m_preferredTrianglesPerChunk{250000U};
};

class MeshBvhBuilder
{
public:
    MeshBvhBuildResult BuildFromChunks(const std::vector<MeshChunkDescriptor>& chunks) const;
private:
    std::uint32_t BuildRecursive(
        const std::vector<MeshChunkDescriptor>& chunks,
        std::vector<std::uint32_t>& chunkIndices,
        std::uint32_t begin,
        std::uint32_t end,
        std::vector<MeshBvhNode>& nodes) const;
};

class MeshLodBuilder
{
public:
    MeshLodBuildResult BuildFromAsset(const LargeMeshAsset& asset) const;
};

class LargeMeshAsyncBuilder
{
public:
    std::future<LargeMeshAccelerationBuildResult> StartBackgroundBuild(const LargeMeshAsset& asset) const;
    LargeMeshAccelerationBuildResult BuildNow(const LargeMeshAsset& asset) const;
};

struct BinaryStlReadResult
{
    ImportDiagnostics diagnostics;
    LargeMeshAsset asset;
};

class BinaryStlFastReader
{
public:
    BinaryStlReadResult ReadBinaryStl(const std::filesystem::path& path, const LargeMeshImportOptions& options = {}) const;
    BinaryStlReadResult CreateM0StubResult(const std::filesystem::path& path) const;
    BinaryStlReadResult CreateSynthetic20MPressureAsset(const std::string& name = "synthetic_20m_pressure.stl", std::uint32_t preferredTrianglesPerChunk = 250000U) const;
};

struct LargeMeshDiagnostics
{
    std::string name;
    std::uint64_t triangleCount{0};
    std::uint32_t chunkCount{0};
    std::uint64_t fileSizeBytes{0};
    std::uint64_t estimatedMemoryBytes{0};
    double importTimeSeconds{0.0};
    BoundingBox3d bounds;
    std::uint32_t previewTriangleCount{0};
    std::uint32_t previewStride{1};
    bool bvhReady{false};
    bool lodReady{false};
    LargeMeshBuildState bvhState{LargeMeshBuildState::NotStarted};
    LargeMeshBuildState lodState{LargeMeshBuildState::NotStarted};
    std::uint32_t bvhNodeCount{0};
    std::uint32_t lodLevelCount{0};
};

LargeMeshDiagnostics InspectLargeMeshAsset(const LargeMeshAsset& asset);
std::string FormatByteSize(std::uint64_t bytes);
std::string ToString(LargeMeshBuildState state);
}
