#pragma once

#include "CAM/BRepCamGeometryProvider.h"
#include "LargeMesh/LargeMesh.h"
#include "Model/CamProject.h"
#include "Post/PostProcessor.h"
#include "Render/RenderCore.h"
#include "Simulation/SimulationStubs.h"

#include <cstddef>
#include <filesystem>
#include <future>
#include <string>
#include <vector>

namespace skylark::cam
{
class ProjectSession
{
public:
    ProjectSession();
    ~ProjectSession();

    ProjectSession(const ProjectSession&) = delete;
    ProjectSession& operator=(const ProjectSession&) = delete;

    const CamProject& GetProject() const noexcept;
    CamProject& GetProject() noexcept;

    void ResetToM0Project();
    void ResetToM1Project();
    void ResetToM2Project();
    void ResetToM3Project();
    void ResetToM4Project();
    void ResetToM5Project();
    void ResetToM6Project();
    void ResetToM7Project();

    void CreateNewCamProject(std::string name);
    std::size_t CreateSetup(std::string name);
    bool SetActiveSetupWcs(WorkCoordinateSystem wcs);
    bool DefineActiveSetupBoxStock(StockDefinition stock);
    bool DefineActiveSetupFixtureStub(FixtureDefinition fixture);
    ToolId AddToolToLibrary(ToolDefinition tool);
    bool SaveProjectJson(const std::filesystem::path& path);
    bool LoadProjectJson(const std::filesystem::path& path);

    ToolpathResult CalculateZLevelToolpath();
    ToolpathResult CalculateParallelFinishingToolpath();
    SimulationResult RunM5Simulation();
    const SimulationResult& GetLastSimulationResult() const noexcept;
    bool HasSimulationResult() const noexcept;

    GCodeProgram GenerateM6GenericIsoProgram();
    GCodeProgram GenerateM6FanucProgram();
    bool ExportM6NcFile(const std::filesystem::path& path);
    OperationSheet GenerateM6OperationSheet();
    const GCodeProgram& GetLastGCodeProgram() const noexcept;
    bool HasGCodeProgram() const noexcept;
    const OperationSheet& GetLastOperationSheet() const noexcept;
    bool HasOperationSheet() const noexcept;

    StepBrepImportResult ImportStepAsBRep(const std::filesystem::path& path);
    StepBrepImportResult CreateM7SyntheticBRepModel();
    ToolpathResult CalculateM7BrepContourToolpath();
    ToolpathResult CalculateM7BrepPocketToolpath();
    ToolpathResult CalculateM7BrepDrillingToolpath();
    bool SelectM7BrepFace(std::uint32_t faceId);
    bool SelectM7BrepEdge(std::uint32_t edgeId);
    const std::vector<BrepModel>& GetBrepModels() const noexcept;
    const BrepModel* GetActiveBrepModel() const noexcept;
    const BrepSelectionState& GetBrepSelection() const noexcept;
    const BrepFeatureDiagnostics& GetLastBrepDiagnostics() const noexcept;
    bool HasBrepDiagnostics() const noexcept;

    BinaryStlReadResult ImportBinaryStl(const std::filesystem::path& path, const LargeMeshImportOptions& options = {});
    BinaryStlReadResult CreateSynthetic20MPressureAsset();
    const std::vector<LargeMeshAsset>& GetLargeMeshAssets() const noexcept;
    const LargeMeshAsset* GetActiveLargeMeshAsset() const noexcept;

    void ScheduleLargeMeshAccelerationBuild(std::size_t assetIndex);
    std::uint32_t PollLargeMeshAccelerationBuilds();
    std::uint32_t DrainLargeMeshAccelerationBuilds();
    std::uint32_t GetPendingAccelerationBuildCount() const;

    void RebuildLargeMeshRenderProxies();
    const std::vector<LargeMeshRenderProxy>& GetLargeMeshRenderProxies() const noexcept;
    GpuUploadQueue& GetGpuUploadQueue() noexcept;
    const GpuUploadQueue& GetGpuUploadQueue() const noexcept;
    GpuUploadQueueSnapshot GetGpuUploadQueueSnapshot() const;
    std::uint32_t PumpGpuUploads(std::uint32_t maxRequestsToUpload);

    void AppendLog(std::string message);
    const std::vector<std::string>& GetLogMessages() const noexcept;

private:
    struct PendingAccelerationBuild
    {
        std::size_t assetIndex{0};
        std::future<LargeMeshAccelerationBuildResult> future;
    };

    void AddImportedMeshAssetToProject(const LargeMeshAsset& asset);
    void AddImportedBrepModelToProject(const BrepModel& model, const BrepFeatureDiagnostics& diagnostics);
    void ApplyAccelerationResult(std::size_t assetIndex, const LargeMeshAccelerationBuildResult& result);
    void ClearAsyncBuilds();

    CamProject m_project;
    std::vector<LargeMeshAsset> m_largeMeshAssets;
    std::vector<LargeMeshRenderProxy> m_largeMeshRenderProxies;
    std::vector<BrepModel> m_brepModels;
    BrepSelectionState m_brepSelection;
    BrepFeatureDiagnostics m_lastBrepDiagnostics;
    bool m_hasBrepDiagnostics{false};
    GpuUploadQueue m_gpuUploadQueue;
    SimulationResult m_lastSimulationResult;
    bool m_hasSimulationResult{false};
    GCodeProgram m_lastGCodeProgram;
    bool m_hasGCodeProgram{false};
    OperationSheet m_lastOperationSheet;
    bool m_hasOperationSheet{false};
    std::vector<PendingAccelerationBuild> m_pendingBuilds;
    std::vector<std::string> m_logMessages;
};
}
