#include "Model/CamProject.h"

#include <algorithm>
#include <utility>

namespace skylark::cam
{
namespace
{
ToolpathDocument CreateDemoToolpath()
{
    ToolpathDocument document;
    document.name = "M1 Demo Toolpath Overlay";
    document.visible = true;
    document.moves = {
        {ToolpathMoveKind::Rapid, {-45.0, -30.0, 42.0}, 6000.0},
        {ToolpathMoveKind::LeadIn, {-45.0, -30.0, 28.0}, 1000.0},
        {ToolpathMoveKind::Feed, {45.0, -30.0, 28.0}, 1200.0},
        {ToolpathMoveKind::Feed, {45.0, -10.0, 24.0}, 1200.0},
        {ToolpathMoveKind::Feed, {-45.0, -10.0, 24.0}, 1200.0},
        {ToolpathMoveKind::Feed, {-45.0, 10.0, 20.0}, 1200.0},
        {ToolpathMoveKind::Feed, {45.0, 10.0, 20.0}, 1200.0},
        {ToolpathMoveKind::Feed, {45.0, 30.0, 18.0}, 1200.0},
        {ToolpathMoveKind::Feed, {-45.0, 30.0, 18.0}, 1200.0},
        {ToolpathMoveKind::Retract, {-45.0, 30.0, 42.0}, 6000.0}};
    return document;
}

CamSetup CreateDefaultCamSetup()
{
    CamSetup setup;
    setup.name = "Setup 1 - 3 Axis Mesh CAM";
    setup.wcs.name = "WCS Top";
    setup.stock.name = "Box Stock 120 x 80 x 40";
    setup.stock.sizeX = 120.0;
    setup.stock.sizeY = 80.0;
    setup.stock.sizeZ = 40.0;
    setup.fixture.name = "Fixture Stub - Vise";
    return setup;
}
}

CamProject::CamProject() = default;
const std::string& CamProject::GetName() const noexcept { return m_name; }
void CamProject::SetName(std::string name) { m_name = std::move(name); }
const std::vector<ImportedModel>& CamProject::GetImportedModels() const noexcept { return m_importedModels; }
std::vector<ImportedModel>& CamProject::GetImportedModels() noexcept { return m_importedModels; }
const std::vector<CamSetup>& CamProject::GetSetups() const noexcept { return m_setups; }
std::vector<CamSetup>& CamProject::GetSetups() noexcept { return m_setups; }
const ToolLibrary& CamProject::GetToolLibrary() const noexcept { return m_toolLibrary; }
ToolLibrary& CamProject::GetToolLibrary() noexcept { return m_toolLibrary; }
const OperationTree& CamProject::GetOperationTree() const noexcept { return m_operationTree; }
OperationTree& CamProject::GetOperationTree() noexcept { return m_operationTree; }
const std::vector<ToolpathDocument>& CamProject::GetToolpathDocuments() const noexcept { return m_toolpathDocuments; }
std::vector<ToolpathDocument>& CamProject::GetToolpathDocuments() noexcept { return m_toolpathDocuments; }
const PostProcessorConfig& CamProject::GetPostConfig() const noexcept { return m_postConfig; }
void CamProject::SetPostConfig(PostProcessorConfig config) { m_postConfig = std::move(config); }
const SimulationSettings& CamProject::GetSimulationSettings() const noexcept { return m_simulationSettings; }
void CamProject::SetSimulationSettings(SimulationSettings settings) { m_simulationSettings = settings; }
void CamProject::AddImportedModel(ImportedModel model) { m_importedModels.push_back(std::move(model)); }
void CamProject::ClearImportedModels() { m_importedModels.clear(); }
void CamProject::RemovePlaceholderModels()
{
    m_importedModels.erase(
        std::remove_if(m_importedModels.begin(), m_importedModels.end(), [](const ImportedModel& model) { return model.placeholder; }),
        m_importedModels.end());
}
void CamProject::AddSetup(CamSetup setup) { m_setups.push_back(std::move(setup)); }
void CamProject::ClearSetups() { m_setups.clear(); }
void CamProject::AddToolpathDocument(ToolpathDocument document) { m_toolpathDocuments.push_back(std::move(document)); }
void CamProject::ClearToolpathDocuments() { m_toolpathDocuments.clear(); }

bool CamProject::UpdateImportedMeshAccelerationStatus(
    const std::string& modelName,
    bool bvhReady,
    bool lodReady,
    std::uint32_t bvhNodeCount,
    std::uint32_t lodLevelCount)
{
    for (ImportedModel& model : m_importedModels)
    {
        if (model.name == modelName && model.geometryKind == CamGeometryKind::Mesh)
        {
            model.meshPart.bvhReady = bvhReady;
            model.meshPart.lodReady = lodReady;
            model.meshPart.bvhNodeCount = bvhNodeCount;
            model.meshPart.lodLevelCount = lodLevelCount;
            return true;
        }
    }
    return false;
}

bool CamProject::UpdateImportedBrepDiagnostics(
    const std::string& modelName,
    std::uint32_t bodyCount,
    std::uint32_t faceCount,
    std::uint32_t edgeCount,
    BoundingBox3d bounds)
{
    for (ImportedModel& model : m_importedModels)
    {
        if (model.name == modelName && model.geometryKind == CamGeometryKind::BRep)
        {
            model.brepPart.bodyCount = bodyCount;
            model.brepPart.faceCount = faceCount;
            model.brepPart.edgeCount = edgeCount;
            model.brepPart.bounds = bounds;
            return true;
        }
    }
    return false;
}

ImportedModel CreatePlaceholderImportedModel()
{
    ImportedModel model;
    model.id = 1;
    model.name = "No model imported yet - use File / Import STL";
    model.sourcePath = "";
    model.geometryKind = CamGeometryKind::Mesh;
    model.placeholder = true;
    model.meshPart.name = "Awaiting Binary STL";
    model.meshPart.triangleCount = 0;
    model.meshPart.chunkCount = 0;
    model.meshPart.bvhReady = false;
    model.meshPart.lodReady = false;
    model.assemblyRoot.name = "Imported Models";
    return model;
}

CamProject CreateM0CamProject()
{
    CamProject project;
    project.SetName("Skylark CAM Project - M0 Skeleton");
    project.AddImportedModel(CreatePlaceholderImportedModel());
    project.AddSetup(CreateDefaultCamSetup());
    project.GetToolLibrary().AddTool({1, "T01 Flat End Mill D10", CutterKind::FlatEndMill, 10.0, 0.0, 32.0, 65.0});
    project.GetToolLibrary().AddTool({2, "T02 Ball End Mill D6", CutterKind::BallEndMill, 6.0, 3.0, 26.0, 58.0});
    project.GetToolLibrary().AddTool({3, "T03 Drill D5", CutterKind::Drill, 5.0, 0.0, 35.0, 72.0});
    project.GetOperationTree().AddOperation({1, "01 Mesh 3D Roughing", CamOperationType::Roughing3D, OperationState::Draft});
    project.GetOperationTree().AddOperation({2, "02 Z-Level Finish", CamOperationType::ZLevelFinishing, OperationState::Draft});
    project.GetOperationTree().AddOperation({3, "03 Parallel Finish", CamOperationType::ParallelFinishing, OperationState::Draft});
    project.GetOperationTree().AddOperation({4, "04 Drilling Stub", CamOperationType::Drilling, OperationState::Draft});
    project.AddToolpathDocument(CreateDemoToolpath());
    return project;
}

CamProject CreateM1CamProject()
{
    CamProject project = CreateM0CamProject();
    project.SetName("Skylark CAM Project - M1 Large STL Validation");
    return project;
}

CamProject CreateM2CamProject()
{
    CamProject project = CreateM1CamProject();
    project.SetName("Skylark CAM Project - M2 LargeMesh RenderProxy Validation");
    return project;
}

CamProject CreateM3CamProject()
{
    CamProject project = CreateM2CamProject();
    project.SetName("Skylark CAM Project - M3 Manufacturing Project");
    return project;
}
}


// M4 keeps M3 manufacturing semantics and enables first Mesh CAM toolpath calculation.
namespace skylark::cam
{
CamProject CreateM4CamProject()
{
    CamProject project = CreateM3CamProject();
    project.SetName("Skylark CAM Project - M4 Mesh CAM Operations");
    project.ClearToolpathDocuments();
    project.GetOperationTree().Clear();
    project.GetOperationTree().AddOperation({1, "01 Mesh 3D Roughing Stub", CamOperationType::Roughing3D, OperationState::Draft});
    project.GetOperationTree().AddOperation({2, "02 Z-Level Finish V1", CamOperationType::ZLevelFinishing, OperationState::Valid});
    project.GetOperationTree().AddOperation({3, "03 Parallel Finish V1", CamOperationType::ParallelFinishing, OperationState::Valid});
    project.GetOperationTree().AddOperation({4, "04 Projection Stub", CamOperationType::Projection, OperationState::Draft});
    project.GetOperationTree().AddOperation({5, "05 Drilling Stub", CamOperationType::Drilling, OperationState::Draft});
    return project;
}
}


// M5 keeps the Mesh CAM operation documents and enables verification/simulation semantics.
namespace skylark::cam
{
CamProject CreateM5CamProject()
{
    CamProject project = CreateM4CamProject();
    project.SetName("Skylark CAM Project - M5 Toolpath Simulation");
    SimulationSettings settings = project.GetSimulationSettings();
    settings.enableToolAnimation = true;
    settings.enableCollisionCheck = true;
    settings.enableStockRemoval = true;
    project.SetSimulationSettings(settings);
    return project;
}
}

// M6 enables real post processing, NC preview/export and operation sheet generation.
namespace skylark::cam
{
CamProject CreateM6CamProject()
{
    CamProject project = CreateM5CamProject();
    project.SetName("Skylark CAM Project - M6 Post Processing");
    PostProcessorConfig config = project.GetPostConfig();
    config.activePost = "Generic ISO 3-Axis";
    config.ncExtension = ".nc";
    project.SetPostConfig(config);
    return project;
}
}


// M7 enables STEP/BRep import and the first exact-geometry CAM channel.
namespace skylark::cam
{
CamProject CreateM7CamProject()
{
    CamProject project = CreateM6CamProject();
    project.SetName("Skylark CAM Project - M7 BRep CAM Channel");
    project.GetOperationTree().Clear();
    project.GetOperationTree().AddOperation({1, "01 Mesh 3D Roughing Stub", CamOperationType::Roughing3D, OperationState::Draft});
    project.GetOperationTree().AddOperation({2, "02 Mesh Z-Level Finish V1", CamOperationType::ZLevelFinishing, OperationState::Valid});
    project.GetOperationTree().AddOperation({3, "03 Mesh Parallel Finish V1", CamOperationType::ParallelFinishing, OperationState::Valid});
    project.GetOperationTree().AddOperation({4, "04 BRep 2.5D Contour V1", CamOperationType::Contour2D, OperationState::Valid});
    project.GetOperationTree().AddOperation({5, "05 BRep Pocket Stub", CamOperationType::Pocket2D, OperationState::Draft});
    project.GetOperationTree().AddOperation({6, "06 BRep Drilling Stub", CamOperationType::Drilling, OperationState::Draft});
    return project;
}
}
