#include "App/ProjectSession.h"

#include "Model/CamProjectJson.h"
#include "CAM/MeshCamGeometryProvider.h"
#include "CAM/BRepCamGeometryProvider.h"
#include "Toolpath/ToolpathPlannerStubs.h"
#include "Simulation/SimulationStubs.h"
#include "Post/PostProcessor.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
ImportedModel CreateImportedModelFromLargeMesh(const LargeMeshAsset& asset)
{
    ImportedModel model;
    model.id = 1000ULL + static_cast<ImportedModelId>(asset.GetTriangleCount() % 1000000ULL);
    model.name = asset.GetName();
    model.sourcePath = asset.GetSourcePath();
    model.geometryKind = CamGeometryKind::Mesh;
    model.meshPart.name = asset.GetName();
    model.meshPart.triangleCount = asset.GetTriangleCount();
    model.meshPart.chunkCount = static_cast<std::uint32_t>(asset.GetChunks().size());
    model.meshPart.bvhReady = asset.IsBvhReady();
    model.meshPart.lodReady = asset.IsLodReady();
    model.meshPart.bvhNodeCount = asset.GetBvhNodeCount();
    model.meshPart.lodLevelCount = asset.GetLodLevelCount();
    model.meshPart.bounds = asset.GetBounds();
    model.assemblyRoot.name = asset.GetName();
    return model;
}


ImportedModel CreateImportedModelFromBrep(const BrepModel& brepModel, const BrepFeatureDiagnostics& diagnostics)
{
    ImportedModel model;
    model.id = 7000ULL + static_cast<ImportedModelId>(diagnostics.faceCount * 31U + diagnostics.edgeCount);
    model.name = brepModel.GetName();
    model.sourcePath = brepModel.GetSourcePath();
    model.geometryKind = CamGeometryKind::BRep;
    model.brepPart.name = brepModel.GetName();
    model.brepPart.bodyCount = diagnostics.bodyCount;
    model.brepPart.faceCount = diagnostics.faceCount;
    model.brepPart.edgeCount = diagnostics.edgeCount;
    model.brepPart.bounds = diagnostics.bounds;
    model.assemblyRoot.name = brepModel.GetName();
    for (const BrepFace& face : brepModel.GetFaces())
    {
        model.sourceEntityMap.push_back({brepModel.GetSourcePath(), "FACE:" + std::to_string(face.id), "BRepFace:" + std::to_string(face.id)});
    }
    for (const BrepEdge& edge : brepModel.GetEdges())
    {
        model.sourceEntityMap.push_back({brepModel.GetSourcePath(), "EDGE:" + std::to_string(edge.id), "BRepEdge:" + std::to_string(edge.id)});
    }
    return model;
}
}

ProjectSession::ProjectSession()
{
    ResetToM7Project();
}

ProjectSession::~ProjectSession()
{
    ClearAsyncBuilds();
}

const CamProject& ProjectSession::GetProject() const noexcept { return m_project; }
CamProject& ProjectSession::GetProject() noexcept { return m_project; }

void ProjectSession::ResetToM0Project()
{
    ClearAsyncBuilds();
    m_project = CreateM0CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M0 CAM project skeleton initialized.");
}

void ProjectSession::ResetToM1Project()
{
    ClearAsyncBuilds();
    m_project = CreateM1CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M1 CAM project initialized: real Binary STL import and interactive 3D viewport are enabled.");
}

void ProjectSession::ResetToM2Project()
{
    ClearAsyncBuilds();
    m_project = CreateM2CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M2 CAM project initialized: Chunked Mesh Asset, LargeMeshRenderProxy, GPU upload queue, background BVH and LOD are enabled.");
}


void ProjectSession::ResetToM3Project()
{
    ClearAsyncBuilds();
    m_project = CreateM3CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M3 CAM project initialized: CamProject, Setup, WCS, Box Stock, Fixture Stub, Tool Library and JSON persistence are enabled.");
}

void ProjectSession::ResetToM4Project()
{
    ClearAsyncBuilds();
    m_project = CreateM4CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M4 CAM project initialized: MeshCamGeometryProvider, ray query, plane slicing, Z height query, Z-Level and Parallel Finishing toolpath V1 are enabled.");
}

void ProjectSession::ResetToM5Project()
{
    ClearAsyncBuilds();
    m_project = CreateM5CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M5 CAM project initialized: toolpath playback, cutter/holder display, collision checking, ZMap/Dexel stock removal and verification report are enabled.");
}



void ProjectSession::ResetToM6Project()
{
    ClearAsyncBuilds();
    m_project = CreateM6CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M6 CAM project initialized: Generic ISO and FANUC post, NC preview/export and operation sheet generation are enabled.");
}


void ProjectSession::ResetToM7Project()
{
    ClearAsyncBuilds();
    m_project = CreateM7CamProject();
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    m_logMessages.clear();
    AppendLog("M7 CAM project initialized: STEP import, BRepCamGeometryProvider, 2.5D contour, pocket/drilling stubs, face/edge selection and diagnostics are enabled.");
}
void ProjectSession::CreateNewCamProject(std::string name)
{
    ClearAsyncBuilds();
    m_project = CreateM7CamProject();
    m_project.SetName(std::move(name));
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    AppendLog("Created new CAM project: " + m_project.GetName());
}

std::size_t ProjectSession::CreateSetup(std::string name)
{
    CamSetup setup = CreateDefaultSetup();
    setup.name = std::move(name);
    if (setup.name.empty())
    {
        setup.name = "Setup " + std::to_string(m_project.GetSetups().size() + 1U);
    }
    setup.wcs.name = "G54 Top";
    setup.wcs.origin = {0.0, 0.0, setup.stock.sizeZ};
    setup.wcs.xAxis = {1.0, 0.0, 0.0};
    setup.wcs.yAxis = {0.0, 1.0, 0.0};
    setup.wcs.zAxis = {0.0, 0.0, 1.0};
    setup.stock.name = "Box Stock 120 x 80 x 40";
    setup.stock.sizeX = 120.0;
    setup.stock.sizeY = 80.0;
    setup.stock.sizeZ = 40.0;
    setup.stock.offset = 2.0;
    setup.fixture.name = "Fixture Stub - Vise";
    setup.fixture.collisionEnabled = true;
    setup.machine.name = "Generic 3-Axis Vertical Mill";
    setup.machine.axisCount = 3;

    m_project.AddSetup(setup);
    const std::size_t index = m_project.GetSetups().size() - 1U;
    AppendLog("Created CAM setup: " + m_project.GetSetups().back().name);
    return index;
}

bool ProjectSession::SetActiveSetupWcs(WorkCoordinateSystem wcs)
{
    if (m_project.GetSetups().empty())
    {
        CreateSetup("Setup 1");
    }
    m_project.GetSetups().back().wcs = std::move(wcs);
    AppendLog("Updated active setup WCS: " + m_project.GetSetups().back().wcs.name);
    return true;
}

bool ProjectSession::DefineActiveSetupBoxStock(StockDefinition stock)
{
    if (m_project.GetSetups().empty())
    {
        CreateSetup("Setup 1");
    }
    m_project.GetSetups().back().stock = std::move(stock);
    AppendLog("Defined active setup box stock: " + m_project.GetSetups().back().stock.name);
    return true;
}

bool ProjectSession::DefineActiveSetupFixtureStub(FixtureDefinition fixture)
{
    if (m_project.GetSetups().empty())
    {
        CreateSetup("Setup 1");
    }
    m_project.GetSetups().back().fixture = std::move(fixture);
    AppendLog("Defined active setup fixture stub: " + m_project.GetSetups().back().fixture.name);
    return true;
}

ToolId ProjectSession::AddToolToLibrary(ToolDefinition tool)
{
    if (tool.id == 0U)
    {
        tool.id = m_project.GetToolLibrary().GetNextAvailableToolId();
    }
    const ToolId id = tool.id;
    m_project.GetToolLibrary().AddTool(std::move(tool));
    AppendLog("Added tool to library: " + m_project.GetToolLibrary().GetTools().back().name);
    return id;
}

bool ProjectSession::SaveProjectJson(const std::filesystem::path& path)
{
    std::string message;
    const bool ok = SaveCamProjectJson(path, m_project, message);
    AppendLog(message);
    return ok;
}

bool ProjectSession::LoadProjectJson(const std::filesystem::path& path)
{
    CamProjectJsonResult result = LoadCamProjectJson(path);
    AppendLog(result.message);
    if (!result.success)
    {
        return false;
    }

    ClearAsyncBuilds();
    m_project = std::move(result.project);
    m_largeMeshAssets.clear();
    m_largeMeshRenderProxies.clear();
    m_brepModels.clear();
    m_brepSelection = {};
    m_lastBrepDiagnostics = {};
    m_hasBrepDiagnostics = false;
    m_gpuUploadQueue.Clear();
    m_hasSimulationResult = false;
    m_lastSimulationResult = {};
    m_hasGCodeProgram = false;
    m_lastGCodeProgram = {};
    m_hasOperationSheet = false;
    m_lastOperationSheet = {};
    AppendLog("Loaded CAM project JSON metadata. LargeMesh GPU/BVH runtime caches are intentionally rebuilt from source files in later milestones.");
    return true;
}



ToolpathResult ProjectSession::CalculateZLevelToolpath()
{
    ToolpathResult result;
    const LargeMeshAsset* asset = GetActiveLargeMeshAsset();
    if (asset == nullptr)
    {
        result.warnings.push_back("M4 Z-Level failed: no active LargeMeshAsset. Import STL first.");
        AppendLog(result.warnings.back());
        return result;
    }
    if (m_project.GetSetups().empty())
    {
        CreateSetup("Setup 1 - Auto Created for M4 Z-Level");
    }

    MeshCamGeometryProvider geometry(*asset);
    MeshToolpathOptions options;
    options.safeZ = std::max(asset->GetBounds().max.z + 12.0, 20.0);
    options.zStep = std::max(asset->GetBounds().Size().z / 6.0, 1.0);
    options.feedRate = 1400.0;

    MeshZLevelToolpathPlanner planner;
    result = planner.Calculate(geometry, m_project.GetSetups().back(), options);
    for (const std::string& warning : result.warnings)
    {
        AppendLog(warning);
    }
    if (result.success)
    {
        m_project.AddToolpathDocument(result.document);
        AppendLog("Calculated M4 Z-Level toolpath and added it to ToolpathDocument list.");
    }
    return result;
}

ToolpathResult ProjectSession::CalculateParallelFinishingToolpath()
{
    ToolpathResult result;
    const LargeMeshAsset* asset = GetActiveLargeMeshAsset();
    if (asset == nullptr)
    {
        result.warnings.push_back("M4 Parallel Finishing failed: no active LargeMeshAsset. Import STL first.");
        AppendLog(result.warnings.back());
        return result;
    }
    if (m_project.GetSetups().empty())
    {
        CreateSetup("Setup 1 - Auto Created for M4 Parallel");
    }

    MeshCamGeometryProvider geometry(*asset);
    MeshToolpathOptions options;
    options.safeZ = std::max(asset->GetBounds().max.z + 12.0, 20.0);
    options.stepover = std::max(asset->GetBounds().Size().y / 8.0, 2.0);
    options.sampleStep = std::max(asset->GetBounds().Size().x / 20.0, 1.0);
    options.feedRate = 1600.0;

    MeshParallelFinishingPlanner planner;
    result = planner.Calculate(geometry, m_project.GetSetups().back(), options);
    for (const std::string& warning : result.warnings)
    {
        AppendLog(warning);
    }
    if (result.success)
    {
        m_project.AddToolpathDocument(result.document);
        AppendLog("Calculated M4 Parallel Finishing toolpath and added it to ToolpathDocument list.");
    }
    return result;
}

SimulationResult ProjectSession::RunM5Simulation()
{
    if (m_project.GetToolpathDocuments().empty())
    {
        const ToolpathResult result = CalculateParallelFinishingToolpath();
        if (!result.success)
        {
            AppendLog("M5 simulation could not auto-create a toolpath document.");
        }
    }

    m_lastSimulationResult = RunM5ToolpathSimulation(m_project);
    m_hasSimulationResult = true;
    AppendLog("M5 simulation report generated.");
    for (const std::string& line : m_lastSimulationResult.report.lines)
    {
        AppendLog(line);
    }
    return m_lastSimulationResult;
}

const SimulationResult& ProjectSession::GetLastSimulationResult() const noexcept
{
    return m_lastSimulationResult;
}

bool ProjectSession::HasSimulationResult() const noexcept
{
    return m_hasSimulationResult;
}

GCodeProgram ProjectSession::GenerateM6GenericIsoProgram()
{
    if (m_project.GetToolpathDocuments().empty())
    {
        const ToolpathResult result = CalculateParallelFinishingToolpath();
        if (!result.success)
        {
            AppendLog("M6 Generic ISO post could not auto-create a toolpath document.");
        }
    }

    PostProcessorOptions options;
    options.programName = "O1001";
    options.safeZ = 60.0;
    options.spindleRpm = 8000.0;
    m_lastGCodeProgram = GenerateGenericIsoGCode(m_project, options);
    m_hasGCodeProgram = true;
    m_lastOperationSheet = GenerateOperationSheet(m_project, &m_lastGCodeProgram);
    m_hasOperationSheet = true;
    AppendLog("M6 Generic ISO NC program generated: " + m_lastGCodeProgram.programName);
    for (const std::string& warning : m_lastGCodeProgram.warnings)
    {
        AppendLog("M6 post warning: " + warning);
    }
    return m_lastGCodeProgram;
}

GCodeProgram ProjectSession::GenerateM6FanucProgram()
{
    if (m_project.GetToolpathDocuments().empty())
    {
        const ToolpathResult result = CalculateParallelFinishingToolpath();
        if (!result.success)
        {
            AppendLog("M6 FANUC post could not auto-create a toolpath document.");
        }
    }

    PostProcessorOptions options;
    options.programName = "O2001";
    options.safeZ = 60.0;
    options.spindleRpm = 9000.0;
    m_lastGCodeProgram = GenerateFanuc3AxisGCode(m_project, options);
    m_hasGCodeProgram = true;
    m_lastOperationSheet = GenerateOperationSheet(m_project, &m_lastGCodeProgram);
    m_hasOperationSheet = true;
    AppendLog("M6 FANUC 3-axis NC program generated: " + m_lastGCodeProgram.programName);
    for (const std::string& warning : m_lastGCodeProgram.warnings)
    {
        AppendLog("M6 post warning: " + warning);
    }
    return m_lastGCodeProgram;
}

bool ProjectSession::ExportM6NcFile(const std::filesystem::path& path)
{
    if (!m_hasGCodeProgram || m_lastGCodeProgram.text.empty())
    {
        GenerateM6GenericIsoProgram();
    }

    std::string message;
    const bool ok = ExportGCodeProgram(m_lastGCodeProgram, path, message);
    AppendLog(message);
    return ok;
}

OperationSheet ProjectSession::GenerateM6OperationSheet()
{
    if (!m_hasGCodeProgram)
    {
        GenerateM6GenericIsoProgram();
    }
    m_lastOperationSheet = GenerateOperationSheet(m_project, &m_lastGCodeProgram);
    m_hasOperationSheet = true;
    AppendLog("M6 operation sheet generated.");
    return m_lastOperationSheet;
}

const GCodeProgram& ProjectSession::GetLastGCodeProgram() const noexcept
{
    return m_lastGCodeProgram;
}

bool ProjectSession::HasGCodeProgram() const noexcept
{
    return m_hasGCodeProgram;
}

const OperationSheet& ProjectSession::GetLastOperationSheet() const noexcept
{
    return m_lastOperationSheet;
}

bool ProjectSession::HasOperationSheet() const noexcept
{
    return m_hasOperationSheet;
}


StepBrepImportResult ProjectSession::ImportStepAsBRep(const std::filesystem::path& path)
{
    StepBrepImporter importer;
    StepBrepImportResult result = importer.ImportStepFile(path);
    for (const std::string& message : result.messages)
    {
        AppendLog(message);
    }
    if (!result.success)
    {
        AppendLog("STEP/BRep import failed: " + path.string());
        return result;
    }

    m_brepModels.push_back(result.model);
    m_lastBrepDiagnostics = result.diagnostics;
    m_hasBrepDiagnostics = true;
    AddImportedBrepModelToProject(m_brepModels.back(), m_lastBrepDiagnostics);
    AppendLog("Imported STEP into BRep CAM channel: " + result.model.GetName());
    AppendLog(m_lastBrepDiagnostics.summary);
    return result;
}

StepBrepImportResult ProjectSession::CreateM7SyntheticBRepModel()
{
    StepBrepImportResult result;
    result.success = true;
    result.model = CreateM7SampleBrepModel();
    result.diagnostics = BuildBrepFeatureDiagnostics(result.model);
    result.messages.push_back("Created synthetic M7 BRep model for contour/pocket/drilling validation.");
    m_brepModels.push_back(result.model);
    m_lastBrepDiagnostics = result.diagnostics;
    m_hasBrepDiagnostics = true;
    AddImportedBrepModelToProject(m_brepModels.back(), m_lastBrepDiagnostics);
    AppendLog(result.messages.front());
    AppendLog(m_lastBrepDiagnostics.summary);
    return result;
}

ToolpathResult ProjectSession::CalculateM7BrepContourToolpath()
{
    ToolpathResult result;
    const BrepModel* model = GetActiveBrepModel();
    if (model == nullptr)
    {
        CreateM7SyntheticBRepModel();
        model = GetActiveBrepModel();
    }
    if (model == nullptr)
    {
        result.warnings.push_back("M7 contour failed: no active BRep model.");
        AppendLog(result.warnings.back());
        return result;
    }
    if (m_project.GetSetups().empty()) { CreateSetup("Setup 1 - Auto Created for M7 BRep Contour"); }
    BRepCamGeometryProvider geometry(*model);
    Brep25DContourPlanner planner;
    BrepToolpathOptions options;
    options.safeZ = std::max(model->GetBounds().max.z + 15.0, 30.0);
    options.contourZ = model->GetBounds().max.z;
    result = planner.Calculate(geometry, m_project.GetSetups().back(), options);
    for (const std::string& warning : result.warnings) { AppendLog(warning); }
    if (result.success)
    {
        m_project.AddToolpathDocument(result.document);
        AppendLog("Calculated M7 BRep 2.5D contour toolpath.");
    }
    return result;
}

ToolpathResult ProjectSession::CalculateM7BrepPocketToolpath()
{
    ToolpathResult result;
    const BrepModel* model = GetActiveBrepModel();
    if (model == nullptr)
    {
        CreateM7SyntheticBRepModel();
        model = GetActiveBrepModel();
    }
    if (model == nullptr)
    {
        result.warnings.push_back("M7 pocket failed: no active BRep model.");
        AppendLog(result.warnings.back());
        return result;
    }
    if (m_project.GetSetups().empty()) { CreateSetup("Setup 1 - Auto Created for M7 Pocket"); }
    BRepCamGeometryProvider geometry(*model);
    BrepPocketPlannerStub planner;
    BrepToolpathOptions options;
    options.safeZ = std::max(model->GetBounds().max.z + 15.0, 30.0);
    options.pocketDepth = std::max(model->GetBounds().Size().z * 0.18, 3.0);
    options.stepover = std::max(std::min(model->GetBounds().Size().x, model->GetBounds().Size().y) / 12.0, 2.0);
    result = planner.Calculate(geometry, m_project.GetSetups().back(), options);
    for (const std::string& warning : result.warnings) { AppendLog(warning); }
    if (result.success)
    {
        m_project.AddToolpathDocument(result.document);
        AppendLog("Calculated M7 BRep pocket stub toolpath.");
    }
    return result;
}

ToolpathResult ProjectSession::CalculateM7BrepDrillingToolpath()
{
    ToolpathResult result;
    const BrepModel* model = GetActiveBrepModel();
    if (model == nullptr)
    {
        CreateM7SyntheticBRepModel();
        model = GetActiveBrepModel();
    }
    if (model == nullptr)
    {
        result.warnings.push_back("M7 drilling failed: no active BRep model.");
        AppendLog(result.warnings.back());
        return result;
    }
    if (m_project.GetSetups().empty()) { CreateSetup("Setup 1 - Auto Created for M7 Drilling"); }
    BRepCamGeometryProvider geometry(*model);
    BrepDrillingPlannerStub planner;
    BrepToolpathOptions options;
    options.safeZ = std::max(model->GetBounds().max.z + 15.0, 30.0);
    result = planner.Calculate(geometry, m_project.GetSetups().back(), options);
    for (const std::string& warning : result.warnings) { AppendLog(warning); }
    if (result.success)
    {
        m_project.AddToolpathDocument(result.document);
        AppendLog("Calculated M7 BRep drilling stub toolpath.");
    }
    return result;
}

bool ProjectSession::SelectM7BrepFace(std::uint32_t faceId)
{
    const BrepModel* model = GetActiveBrepModel();
    if (model == nullptr) { return false; }
    BRepCamGeometryProvider geometry(*model);
    const bool ok = geometry.SelectFace(faceId);
    if (ok)
    {
        m_brepSelection.faceIds = geometry.GetSelection().faceIds;
        AppendLog("Selected BRep face id=" + std::to_string(faceId));
    }
    return ok;
}

bool ProjectSession::SelectM7BrepEdge(std::uint32_t edgeId)
{
    const BrepModel* model = GetActiveBrepModel();
    if (model == nullptr) { return false; }
    BRepCamGeometryProvider geometry(*model);
    const bool ok = geometry.SelectEdge(edgeId);
    if (ok)
    {
        m_brepSelection.edgeIds = geometry.GetSelection().edgeIds;
        AppendLog("Selected BRep edge id=" + std::to_string(edgeId));
    }
    return ok;
}

const std::vector<BrepModel>& ProjectSession::GetBrepModels() const noexcept { return m_brepModels; }

const BrepModel* ProjectSession::GetActiveBrepModel() const noexcept
{
    if (m_brepModels.empty()) { return nullptr; }
    return &m_brepModels.back();
}

const BrepSelectionState& ProjectSession::GetBrepSelection() const noexcept { return m_brepSelection; }
const BrepFeatureDiagnostics& ProjectSession::GetLastBrepDiagnostics() const noexcept { return m_lastBrepDiagnostics; }
bool ProjectSession::HasBrepDiagnostics() const noexcept { return m_hasBrepDiagnostics; }

BinaryStlReadResult ProjectSession::ImportBinaryStl(const std::filesystem::path& path, const LargeMeshImportOptions& options)
{
    BinaryStlFastReader reader;
    BinaryStlReadResult result = reader.ReadBinaryStl(path, options);
    for (const ImportDiagnosticMessage& message : result.diagnostics.messages)
    {
        AppendLog(message.severity + ": " + message.text);
    }
    if (!result.diagnostics.success)
    {
        AppendLog("Binary STL import failed: " + path.string());
        return result;
    }

    const LargeMeshDiagnostics diagnostics = InspectLargeMeshAsset(result.asset);
    std::ostringstream stream;
    stream << "Imported Binary STL into Chunked Mesh Asset: " << diagnostics.name
           << ", triangles=" << diagnostics.triangleCount
           << ", chunks=" << diagnostics.chunkCount
           << ", preview=" << diagnostics.previewTriangleCount
           << ", file=" << FormatByteSize(diagnostics.fileSizeBytes)
           << ", estimated GPU payload=" << FormatByteSize(diagnostics.estimatedMemoryBytes)
           << ", time=" << diagnostics.importTimeSeconds << "s";
    AppendLog(stream.str());

    m_largeMeshAssets.push_back(result.asset);
    const std::size_t assetIndex = m_largeMeshAssets.size() - 1U;
    AddImportedMeshAssetToProject(m_largeMeshAssets.back());
    ScheduleLargeMeshAccelerationBuild(assetIndex);
    RebuildLargeMeshRenderProxies();
    return result;
}

BinaryStlReadResult ProjectSession::CreateSynthetic20MPressureAsset()
{
    BinaryStlFastReader reader;
    BinaryStlReadResult result = reader.CreateSynthetic20MPressureAsset();
    for (const ImportDiagnosticMessage& message : result.diagnostics.messages)
    {
        AppendLog(message.severity + ": " + message.text);
    }
    m_largeMeshAssets.push_back(result.asset);
    const std::size_t assetIndex = m_largeMeshAssets.size() - 1U;
    AddImportedMeshAssetToProject(m_largeMeshAssets.back());
    ScheduleLargeMeshAccelerationBuild(assetIndex);
    RebuildLargeMeshRenderProxies();
    AppendLog("M2 synthetic 20M STL pressure entry is active: chunk table + render proxies + GPU queue + async BVH/LOD.");
    return result;
}

const std::vector<LargeMeshAsset>& ProjectSession::GetLargeMeshAssets() const noexcept { return m_largeMeshAssets; }

const LargeMeshAsset* ProjectSession::GetActiveLargeMeshAsset() const noexcept
{
    if (m_largeMeshAssets.empty()) { return nullptr; }
    return &m_largeMeshAssets.back();
}

void ProjectSession::ScheduleLargeMeshAccelerationBuild(std::size_t assetIndex)
{
    if (assetIndex >= m_largeMeshAssets.size()) { return; }
    LargeMeshAsset& asset = m_largeMeshAssets[assetIndex];
    asset.MarkBvhQueued();
    asset.MarkLodQueued();
    LargeMeshAsyncBuilder builder;
    PendingAccelerationBuild pending;
    pending.assetIndex = assetIndex;
    pending.future = builder.StartBackgroundBuild(asset);
    m_pendingBuilds.push_back(std::move(pending));
    AppendLog("Queued background BVH/LOD build for LargeMeshAsset: " + asset.GetName());
}

std::uint32_t ProjectSession::PollLargeMeshAccelerationBuilds()
{
    std::uint32_t completed = 0U;
    auto it = m_pendingBuilds.begin();
    while (it != m_pendingBuilds.end())
    {
        const std::future_status status = it->future.wait_for(std::chrono::seconds(0));
        if (status == std::future_status::ready)
        {
            LargeMeshAccelerationBuildResult result = it->future.get();
            ApplyAccelerationResult(it->assetIndex, result);
            it = m_pendingBuilds.erase(it);
            ++completed;
        }
        else
        {
            ++it;
        }
    }
    if (completed > 0U)
    {
        RebuildLargeMeshRenderProxies();
    }
    return completed;
}

std::uint32_t ProjectSession::DrainLargeMeshAccelerationBuilds()
{
    std::uint32_t completed = 0U;
    for (PendingAccelerationBuild& pending : m_pendingBuilds)
    {
        LargeMeshAccelerationBuildResult result = pending.future.get();
        ApplyAccelerationResult(pending.assetIndex, result);
        ++completed;
    }
    m_pendingBuilds.clear();
    if (completed > 0U)
    {
        RebuildLargeMeshRenderProxies();
    }
    return completed;
}

std::uint32_t ProjectSession::GetPendingAccelerationBuildCount() const
{
    return static_cast<std::uint32_t>(m_pendingBuilds.size());
}

void ProjectSession::RebuildLargeMeshRenderProxies()
{
    RenderProxyBuilder builder;
    m_largeMeshRenderProxies = builder.BuildLargeMeshRenderProxies(m_largeMeshAssets);
    m_gpuUploadQueue.Clear();
    for (const LargeMeshRenderProxy& proxy : m_largeMeshRenderProxies)
    {
        m_gpuUploadQueue.EnqueueLargeMeshProxy(proxy);
    }
}

const std::vector<LargeMeshRenderProxy>& ProjectSession::GetLargeMeshRenderProxies() const noexcept { return m_largeMeshRenderProxies; }
GpuUploadQueue& ProjectSession::GetGpuUploadQueue() noexcept { return m_gpuUploadQueue; }
const GpuUploadQueue& ProjectSession::GetGpuUploadQueue() const noexcept { return m_gpuUploadQueue; }
GpuUploadQueueSnapshot ProjectSession::GetGpuUploadQueueSnapshot() const { return m_gpuUploadQueue.CreateSnapshot(); }

std::uint32_t ProjectSession::PumpGpuUploads(std::uint32_t maxRequestsToUpload)
{
    const std::uint32_t uploaded = m_gpuUploadQueue.PumpUploads(maxRequestsToUpload);
    if (uploaded > 0U)
    {
        std::ostringstream stream;
        stream << "GPU Upload Queue pumped " << uploaded << " chunk upload request(s).";
        AppendLog(stream.str());
    }
    return uploaded;
}

void ProjectSession::AppendLog(std::string message)
{
    m_logMessages.push_back(std::move(message));
}

const std::vector<std::string>& ProjectSession::GetLogMessages() const noexcept { return m_logMessages; }

void ProjectSession::AddImportedMeshAssetToProject(const LargeMeshAsset& asset)
{
    m_project.RemovePlaceholderModels();
    m_project.AddImportedModel(CreateImportedModelFromLargeMesh(asset));
}


void ProjectSession::AddImportedBrepModelToProject(const BrepModel& model, const BrepFeatureDiagnostics& diagnostics)
{
    m_project.RemovePlaceholderModels();
    m_project.AddImportedModel(CreateImportedModelFromBrep(model, diagnostics));
}

void ProjectSession::ApplyAccelerationResult(std::size_t assetIndex, const LargeMeshAccelerationBuildResult& result)
{
    if (assetIndex >= m_largeMeshAssets.size()) { return; }
    LargeMeshAsset& asset = m_largeMeshAssets[assetIndex];
    asset.ApplyAccelerationBuildResult(result);
    m_project.UpdateImportedMeshAccelerationStatus(
        asset.GetName(),
        asset.IsBvhReady(),
        asset.IsLodReady(),
        asset.GetBvhNodeCount(),
        asset.GetLodLevelCount());

    std::ostringstream stream;
    stream << "Background acceleration ready for " << asset.GetName()
           << ": BVH=" << ToString(asset.GetBvhState())
           << " nodes=" << asset.GetBvhNodeCount()
           << ", LOD=" << ToString(asset.GetLodState())
           << " levels=" << asset.GetLodLevelCount()
           << ", totalBuild=" << result.totalBuildTimeSeconds << "s";
    AppendLog(stream.str());
}

void ProjectSession::ClearAsyncBuilds()
{
    for (PendingAccelerationBuild& pending : m_pendingBuilds)
    {
        if (pending.future.valid())
        {
            pending.future.wait();
        }
    }
    m_pendingBuilds.clear();
}
}
