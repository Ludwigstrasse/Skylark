#include "Diagnostics/BenchmarkReport.h"

#include "LargeMesh/LargeMesh.h"

#include <sstream>
#include <utility>

namespace skylark::cam
{
void BenchmarkReport::AddCounter(std::string name, std::string value)
{
    counters.push_back({std::move(name), std::move(value)});
}

BenchmarkReport CreateM0BenchmarkReport()
{
    BenchmarkReport report;
    report.AddCounter("Milestone", "M0 shell only");
    report.AddCounter("Viewport", "SkylarkViewportWidget installed");
    report.AddCounter("Data Exchange", "FormatRegistry and ImportSession dry-run APIs installed");
    report.AddCounter("Large Mesh", "Chunked asset API installed; real Binary STL parser starts in M1");
    report.AddCounter("CAM", "CamProject, Setup, WCS, Stock, ToolLibrary, OperationTree installed");
    return report;
}

BenchmarkReport CreateM1BenchmarkReport(const LargeMeshAsset* activeMesh)
{
    BenchmarkReport report;
    report.AddCounter("Milestone", "M1 real 3D viewport + Binary STL large mesh import");
    report.AddCounter("Import Pipeline", "BinaryStlFastReader -> LargeMeshAsset -> CamProject -> SkylarkViewportWidget");
    report.AddCounter("BRep Conversion", "Disabled for STL by design");
    report.AddCounter("Viewport Controls", "Fit / Rotate / Pan / Zoom");
    report.AddCounter("M2 Boundary", "BVH / LOD / GPU upload queue are intentionally still stubs");
    if (activeMesh == nullptr)
    {
        report.AddCounter("Active STL", "None");
        return report;
    }

    const LargeMeshDiagnostics diagnostics = InspectLargeMeshAsset(*activeMesh);
    report.AddCounter("Active STL", diagnostics.name);
    report.AddCounter("Triangle Count", std::to_string(diagnostics.triangleCount));
    report.AddCounter("Chunk Count", std::to_string(diagnostics.chunkCount));
    report.AddCounter("Preview Triangle Count", std::to_string(diagnostics.previewTriangleCount));
    report.AddCounter("Preview Stride", std::to_string(diagnostics.previewStride));
    report.AddCounter("File Size", FormatByteSize(diagnostics.fileSizeBytes));
    report.AddCounter("Estimated GPU Payload", FormatByteSize(diagnostics.estimatedMemoryBytes));
    std::ostringstream stream;
    stream << diagnostics.importTimeSeconds << " s";
    report.AddCounter("Import Time", stream.str());
    return report;
}

BenchmarkReport CreateM2BenchmarkReport(const LargeMeshAsset* activeMesh, const GpuUploadQueueSnapshot& uploadQueue, std::uint32_t pendingAccelerationBuilds)
{
    BenchmarkReport report;
    report.AddCounter("Milestone", "M2 LargeMesh + Skylark RenderProxy");
    report.AddCounter("STL to BRep", "Disabled");
    report.AddCounter("Per-Triangle Objects", "Disabled; chunk descriptors and preview payload only");
    report.AddCounter("GPU Upload Requests", std::to_string(uploadQueue.totalRequests));
    report.AddCounter("GPU Pending Requests", std::to_string(uploadQueue.pendingRequests));
    report.AddCounter("GPU Uploaded Requests", std::to_string(uploadQueue.uploadedRequests));
    report.AddCounter("GPU Pending Bytes", FormatByteSize(uploadQueue.pendingBytes));
    report.AddCounter("GPU Uploaded Bytes", FormatByteSize(uploadQueue.uploadedBytes));
    report.AddCounter("Background BVH/LOD Pending Jobs", std::to_string(pendingAccelerationBuilds));
    if (activeMesh == nullptr)
    {
        report.AddCounter("Active STL", "None");
        report.AddCounter("20M Pressure Entry", "Use File / 20M STL Pressure or headless --m2-pressure20m");
        return report;
    }

    const LargeMeshDiagnostics diagnostics = InspectLargeMeshAsset(*activeMesh);
    report.AddCounter("Active STL", diagnostics.name);
    report.AddCounter("Triangle Count", std::to_string(diagnostics.triangleCount));
    report.AddCounter("Chunk Count", std::to_string(diagnostics.chunkCount));
    report.AddCounter("Preview Triangle Count", std::to_string(diagnostics.previewTriangleCount));
    report.AddCounter("Preview Stride", std::to_string(diagnostics.previewStride));
    report.AddCounter("File Size", FormatByteSize(diagnostics.fileSizeBytes));
    report.AddCounter("Estimated GPU Payload", FormatByteSize(diagnostics.estimatedMemoryBytes));
    report.AddCounter("BVH State", ToString(diagnostics.bvhState));
    report.AddCounter("BVH Node Count", std::to_string(diagnostics.bvhNodeCount));
    report.AddCounter("LOD State", ToString(diagnostics.lodState));
    report.AddCounter("LOD Level Count", std::to_string(diagnostics.lodLevelCount));
    std::ostringstream stream;
    stream << diagnostics.importTimeSeconds << " s";
    report.AddCounter("Import Time", stream.str());
    return report;
}
}
