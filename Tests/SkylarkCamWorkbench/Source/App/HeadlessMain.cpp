#include "App/CommandRegistry.h"
#include "App/ProjectSession.h"
#include "CAM/MeshCamGeometryProvider.h"
#include "CAM/BRepCamGeometryProvider.h"
#include "Common/CamCore.h"
#include "DataExchange/DataExchange.h"
#include "Diagnostics/BenchmarkReport.h"
#include "Render/RenderCore.h"
#include "Simulation/SimulationStubs.h"
#include "Post/PostProcessor.h"

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
void WriteFloat32LE(std::ofstream& output, float value)
{
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    output.put(static_cast<char>(bytes[0]));
    output.put(static_cast<char>(bytes[1]));
    output.put(static_cast<char>(bytes[2]));
    output.put(static_cast<char>(bytes[3]));
}

void WriteUInt32LE(std::ofstream& output, std::uint32_t value)
{
    output.put(static_cast<char>(value & 0xFFU));
    output.put(static_cast<char>((value >> 8U) & 0xFFU));
    output.put(static_cast<char>((value >> 16U) & 0xFFU));
    output.put(static_cast<char>((value >> 24U) & 0xFFU));
}

void WriteTriangle(std::ofstream& output, const std::array<float, 3>& normal, const std::array<float, 3>& a, const std::array<float, 3>& b, const std::array<float, 3>& c)
{
    for (float v : normal) { WriteFloat32LE(output, v); }
    for (float v : a) { WriteFloat32LE(output, v); }
    for (float v : b) { WriteFloat32LE(output, v); }
    for (float v : c) { WriteFloat32LE(output, v); }
    output.put('\0');
    output.put('\0');
}

std::filesystem::path WriteSmokeBinaryStl()
{
    const std::filesystem::path path = std::filesystem::temp_directory_path() / "skylark_cam_workbench_m2_smoke.stl";
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    std::array<char, 80> header{};
    const char text[] = "SkylarkCamWorkbench M2 Binary STL smoke mesh";
    for (std::size_t i = 0; i < sizeof(text) - 1U && i < header.size(); ++i) { header[i] = text[i]; }
    output.write(header.data(), static_cast<std::streamsize>(header.size()));
    WriteUInt32LE(output, 4U);
    WriteTriangle(output, {0.0F, 0.0F, 1.0F}, {-20.0F, -20.0F, 0.0F}, {20.0F, -20.0F, 0.0F}, {20.0F, 20.0F, 0.0F});
    WriteTriangle(output, {0.0F, 0.0F, 1.0F}, {-20.0F, -20.0F, 0.0F}, {20.0F, 20.0F, 0.0F}, {-20.0F, 20.0F, 0.0F});
    WriteTriangle(output, {0.0F, 0.0F, 1.0F}, {-10.0F, -10.0F, 8.0F}, {10.0F, -10.0F, 8.0F}, {10.0F, 10.0F, 8.0F});
    WriteTriangle(output, {0.0F, 0.0F, 1.0F}, {-10.0F, -10.0F, 8.0F}, {10.0F, 10.0F, 8.0F}, {-10.0F, 10.0F, 8.0F});
    return path;
}


std::filesystem::path WriteSmokeStepFile()
{
    const std::filesystem::path path = std::filesystem::temp_directory_path() / "skylark_cam_workbench_m7_smoke.step";
    std::ofstream output(path, std::ios::trunc);
    output << "ISO-10303-21;\n";
    output << "HEADER;FILE_DESCRIPTION(('SkylarkCamWorkbench M7 smoke STEP'),'2;1');ENDSEC;\n";
    output << "DATA;\n";
    output << "#1=CARTESIAN_POINT('',(-60.0,-40.0,0.0));\n";
    output << "#2=CARTESIAN_POINT('',(60.0,-40.0,0.0));\n";
    output << "#3=CARTESIAN_POINT('',(60.0,40.0,0.0));\n";
    output << "#4=CARTESIAN_POINT('',(-60.0,40.0,0.0));\n";
    output << "#5=CARTESIAN_POINT('',(-60.0,-40.0,32.0));\n";
    output << "#6=CARTESIAN_POINT('',(60.0,-40.0,32.0));\n";
    output << "#7=CARTESIAN_POINT('',(60.0,40.0,32.0));\n";
    output << "#8=CARTESIAN_POINT('',(-60.0,40.0,32.0));\n";
    output << "#20=ADVANCED_FACE('',(),#100,.T.);\n";
    output << "#21=ADVANCED_FACE('',(),#101,.T.);\n";
    output << "#30=EDGE_CURVE('',#1,#2,#200,.T.);\n";
    output << "#31=EDGE_CURVE('',#2,#3,#201,.T.);\n";
    output << "#40=CIRCLE('',#300,5.0);\n";
    output << "ENDSEC;END-ISO-10303-21;\n";
    return path;
}

bool HasArg(int argc, char** argv, const std::string& arg)
{
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == arg) { return true; }
    }
    return false;
}

std::filesystem::path GetArgPathAfter(int argc, char** argv, const std::string& arg)
{
    for (int i = 1; i + 1 < argc; ++i)
    {
        if (argv[i] == arg) { return std::filesystem::path(argv[i + 1]); }
    }
    return {};
}
}

int main(int argc, char** argv)
{
    using namespace skylark::cam;
    ProjectSession session;
    CommandRegistry commands = CreateDefaultCommandRegistry();
    FormatRegistry formats;

    LargeMeshImportOptions options;
    options.maxPreviewTriangles = 100U;

    BinaryStlReadResult importResult;
    std::filesystem::path importedPath;
    const bool pressureMode = HasArg(argc, argv, "--m2-pressure20m");
    const std::filesystem::path pressurePath = GetArgPathAfter(argc, argv, "--m2-pressure20m");
    if (pressureMode && !pressurePath.empty() && std::filesystem::exists(pressurePath))
    {
        options.maxPreviewTriangles = 300000U;
        importedPath = pressurePath;
        importResult = session.ImportBinaryStl(pressurePath, options);
    }
    else if (pressureMode)
    {
        importResult = session.CreateSynthetic20MPressureAsset();
        importedPath = "synthetic://20m-binary-stl-pressure";
    }
    else
    {
        importedPath = WriteSmokeBinaryStl();
        importResult = session.ImportBinaryStl(importedPath, options);
    }

    const std::uint32_t completed = session.DrainLargeMeshAccelerationBuilds();
    session.RebuildLargeMeshRenderProxies();
    const std::uint32_t uploaded = session.PumpGpuUploads(100000U);

    WorkCoordinateSystem m3Wcs;
    m3Wcs.name = "G54 M3 Validation Top";
    m3Wcs.origin = {10.0, 20.0, 50.0};
    m3Wcs.xAxis = {1.0, 0.0, 0.0};
    m3Wcs.yAxis = {0.0, 1.0, 0.0};
    m3Wcs.zAxis = {0.0, 0.0, 1.0};
    session.SetActiveSetupWcs(m3Wcs);

    StockDefinition m3Stock;
    m3Stock.name = "M3 Box Stock 160 x 100 x 50";
    m3Stock.sizeX = 160.0;
    m3Stock.sizeY = 100.0;
    m3Stock.sizeZ = 50.0;
    m3Stock.offset = 3.0;
    session.DefineActiveSetupBoxStock(m3Stock);

    FixtureDefinition m3Fixture;
    m3Fixture.name = "M3 Fixture Stub - Modular Vise";
    m3Fixture.collisionEnabled = true;
    session.DefineActiveSetupFixtureStub(m3Fixture);

    ToolDefinition m3Tool;
    m3Tool.id = 0;
    m3Tool.name = "M3 Validation Flat End Mill D12";
    m3Tool.kind = CutterKind::FlatEndMill;
    m3Tool.diameter = 12.0;
    m3Tool.cornerRadius = 0.0;
    m3Tool.fluteLength = 36.0;
    m3Tool.gaugeLength = 72.0;
    const ToolId addedToolId = session.AddToolToLibrary(m3Tool);

    const std::filesystem::path projectJsonPath = std::filesystem::temp_directory_path() / "skylark_cam_workbench_m3_project.skcam.json";
    const bool savedProject = session.SaveProjectJson(projectJsonPath);
    ProjectSession loadedSession;
    const bool loadedProject = loadedSession.LoadProjectJson(projectJsonPath);
    const CamProject& loadedCamProject = loadedSession.GetProject();
    const bool loadedSetupOk = !loadedCamProject.GetSetups().empty()
        && loadedCamProject.GetSetups().back().wcs.name == "G54 M3 Validation Top"
        && loadedCamProject.GetSetups().back().stock.sizeX == 160.0
        && loadedCamProject.GetSetups().back().fixture.name == "M3 Fixture Stub - Modular Vise";
    const bool loadedToolOk = loadedCamProject.GetToolLibrary().GetTools().size() >= session.GetProject().GetToolLibrary().GetTools().size();

    SkylarkSceneAdapter adapter;
    const SceneAdapterSnapshot snapshot = adapter.CreateSnapshot(session.GetProject(), session.GetLargeMeshAssets(), session.GetGpuUploadQueue());
    const BenchmarkReport report = CreateM2BenchmarkReport(session.GetActiveLargeMeshAsset(), session.GetGpuUploadQueueSnapshot(), session.GetPendingAccelerationBuildCount());
    const LargeMeshAsset* asset = session.GetActiveLargeMeshAsset();

    bool rayQueryOk = false;
    bool planeSlicingOk = false;
    bool heightQueryOk = false;
    std::size_t sectionCurveCount = 0U;
    double heightZ = 0.0;
    ToolpathResult zLevelResult;
    ToolpathResult parallelResult;
    if (asset != nullptr)
    {
        MeshCamGeometryProvider geometry(*asset);
        CamIntersectionResult rayHit;
        Ray3d ray;
        ray.origin = Vec3d{0.0, 0.0, asset->GetBounds().max.z + 100.0};
        ray.direction = Vec3d{0.0, 0.0, -1.0};
        rayQueryOk = geometry.RayIntersect(ray, rayHit);

        Plane3d plane;
        plane.origin = Vec3d{0.0, 0.0, asset->GetBounds().min.z};
        plane.normal = Vec3d{0.0, 0.0, 1.0};
        std::vector<Polyline3d> sectionCurves;
        planeSlicingOk = geometry.IntersectPlane(plane, sectionCurves);
        sectionCurveCount = sectionCurves.size();

        heightQueryOk = geometry.QueryHeightAlongZ(0.0, 0.0, heightZ);
        zLevelResult = session.CalculateZLevelToolpath();
        parallelResult = session.CalculateParallelFinishingToolpath();
    }

    const std::filesystem::path stepPath = WriteSmokeStepFile();
    const StepBrepImportResult stepResult = session.ImportStepAsBRep(stepPath);
    const BrepModel* activeBrep = session.GetActiveBrepModel();
    bool brepRayOk = false;
    bool brepPlaneOk = false;
    bool brepHeightOk = false;
    bool brepFaceSelectionOk = false;
    bool brepEdgeSelectionOk = false;
    double brepHeightZ = 0.0;
    std::size_t brepSectionCount = 0U;
    ToolpathResult brepContourResult;
    ToolpathResult brepPocketResult;
    ToolpathResult brepDrillingResult;
    if (activeBrep != nullptr)
    {
        BRepCamGeometryProvider brepGeometry(*activeBrep);
        CamIntersectionResult brepHit;
        Ray3d brepRay;
        brepRay.origin = Vec3d{0.0, 0.0, activeBrep->GetBounds().max.z + 100.0};
        brepRay.direction = Vec3d{0.0, 0.0, -1.0};
        brepRayOk = brepGeometry.RayIntersect(brepRay, brepHit);
        Plane3d brepPlane;
        brepPlane.origin = Vec3d{0.0, 0.0, activeBrep->GetBounds().max.z};
        brepPlane.normal = Vec3d{0.0, 0.0, 1.0};
        std::vector<Polyline3d> brepSections;
        brepPlaneOk = brepGeometry.IntersectPlane(brepPlane, brepSections);
        brepSectionCount = brepSections.size();
        brepHeightOk = brepGeometry.QueryHeightAlongZ(0.0, 0.0, brepHeightZ);
        brepFaceSelectionOk = session.SelectM7BrepFace(2U);
        brepEdgeSelectionOk = session.SelectM7BrepEdge(1U);
        brepContourResult = session.CalculateM7BrepContourToolpath();
        brepPocketResult = session.CalculateM7BrepPocketToolpath();
        brepDrillingResult = session.CalculateM7BrepDrillingToolpath();
    }

    const SimulationResult simulationResult = session.RunM5Simulation();


    const GCodeProgram genericProgram = session.GenerateM6GenericIsoProgram();
    const GCodeProgram fanucProgram = session.GenerateM6FanucProgram();
    const OperationSheet operationSheet = session.GenerateM6OperationSheet();
    const std::filesystem::path ncOutputPath = std::filesystem::temp_directory_path() / "skylark_cam_workbench_m6_fanuc_setup1.nc";
    const bool exportedNc = session.ExportM6NcFile(ncOutputPath);
    const std::string ncPreview = GenerateNcPreview(session.GetLastGCodeProgram(), 40U);
    const bool ok =
        !commands.GetCommands().empty() &&
        formats.FindByExtension("stl") != nullptr &&
        importResult.diagnostics.success &&
        asset != nullptr &&
        asset->GetTriangleCount() > 0ULL &&
        asset->IsBvhReady() &&
        asset->IsLodReady() &&
        !session.GetProject().GetImportedModels().empty() &&
        !session.GetProject().GetSetups().empty() &&
        !session.GetProject().GetToolLibrary().GetTools().empty() &&
        !session.GetProject().GetOperationTree().GetOperations().empty() &&
        rayQueryOk &&
        planeSlicingOk &&
        heightQueryOk &&
        zLevelResult.success &&
        parallelResult.success &&
        session.GetProject().GetToolpathDocuments().size() >= 2U &&
        simulationResult.success &&
        simulationResult.playback.loaded &&
        simulationResult.collision.executed &&
        simulationResult.zmapRemoval.executed &&
        simulationResult.dexelRemoval.executed &&
        simulationResult.report.generated &&
        genericProgram.generated &&
        fanucProgram.generated &&
        session.HasGCodeProgram() &&
        !session.GetLastGCodeProgram().text.empty() &&
        session.GetLastGCodeProgram().postId == "fanuc_3x" &&
        exportedNc &&
        std::filesystem::exists(ncOutputPath) &&
        operationSheet.generated &&
        stepResult.success &&
        activeBrep != nullptr &&
        session.HasBrepDiagnostics() &&
        session.GetLastBrepDiagnostics().valid &&
        session.GetLastBrepDiagnostics().faceCount >= 6U &&
        session.GetLastBrepDiagnostics().edgeCount >= 12U &&
        brepRayOk &&
        brepPlaneOk &&
        brepHeightOk &&
        brepFaceSelectionOk &&
        brepEdgeSelectionOk &&
        brepContourResult.success &&
        brepPocketResult.success &&
        brepDrillingResult.success &&
        !operationSheet.text.empty() &&
        !ncPreview.empty() &&
        simulationResult.zmapRemoval.removedCells > 0ULL &&
        simulationResult.dexelRemoval.removedCells > 0ULL &&
        addedToolId > 0ULL &&
        savedProject &&
        loadedProject &&
        loadedSetupOk &&
        loadedToolOk &&
        !snapshot.proxies.empty() &&
        !snapshot.largeMeshProxies.empty() &&
        snapshot.uploadQueue.uploadedRequests > 0ULL &&
        !report.counters.empty();

    std::cout << GetWorkbenchVersionText() << '\n';
    std::cout << "Commands              : " << commands.GetCommands().size() << '\n';
    std::cout << "Formats               : " << formats.GetFormats().size() << '\n';
    std::cout << "Imported STL          : " << importedPath.string() << '\n';
    std::cout << "Imported triangles    : " << importResult.diagnostics.importedTriangleCount << '\n';
    std::cout << "Preview triangles     : " << (asset != nullptr ? asset->GetPreviewTriangles().size() : 0U) << '\n';
    std::cout << "Mesh chunks           : " << (asset != nullptr ? asset->GetChunks().size() : 0U) << '\n';
    std::cout << "LargeMeshRenderProxy  : " << snapshot.largeMeshProxies.size() << '\n';
    std::cout << "GPU upload requests   : " << snapshot.uploadQueue.totalRequests << '\n';
    std::cout << "GPU uploaded requests : " << snapshot.uploadQueue.uploadedRequests << '\n';
    std::cout << "BVH jobs completed    : " << completed << '\n';
    std::cout << "BVH ready             : " << (asset != nullptr && asset->IsBvhReady() ? "yes" : "no") << '\n';
    std::cout << "LOD ready             : " << (asset != nullptr && asset->IsLodReady() ? "yes" : "no") << '\n';
    std::cout << "GPU pump uploaded     : " << uploaded << '\n';
    std::cout << "M3 project JSON       : " << projectJsonPath.string() << '\n';
    std::cout << "M3 project saved      : " << (savedProject ? "yes" : "no") << '\n';
    std::cout << "M3 project loaded     : " << (loadedProject ? "yes" : "no") << '\n';
    std::cout << "M3 loaded setups      : " << loadedCamProject.GetSetups().size() << '\n';
    std::cout << "M3 loaded tools       : " << loadedCamProject.GetToolLibrary().GetTools().size() << '\n';
    std::cout << "M3 added tool id      : " << addedToolId << '\n';
    std::cout << "M4 ray query          : " << (rayQueryOk ? "yes" : "no") << '\n';
    std::cout << "M4 plane slicing      : " << (planeSlicingOk ? "yes" : "no") << " sections=" << sectionCurveCount << '\n';
    std::cout << "M4 height query       : " << (heightQueryOk ? "yes" : "no") << " z=" << heightZ << '\n';
    std::cout << "M4 Z-Level moves      : " << zLevelResult.document.moves.size() << '\n';
    std::cout << "M4 Parallel moves     : " << parallelResult.document.moves.size() << '\n';
    std::cout << "M4 Toolpath documents : " << session.GetProject().GetToolpathDocuments().size() << '\n';
    std::cout << "M5 playback loaded    : " << (simulationResult.playback.loaded ? "yes" : "no") << '\n';
    std::cout << "M5 playback move      : " << simulationResult.playback.currentMoveIndex << "/" << simulationResult.playback.totalMoves << '\n';
    std::cout << "M5 tool entity        : " << simulationResult.toolAssembly.cutter.name << '\n';
    std::cout << "M5 holder diameter    : " << simulationResult.toolAssembly.holderDiameter << '\n';
    std::cout << "M5 collision executed : " << (simulationResult.collision.executed ? "yes" : "no") << '\n';
    std::cout << "M5 collision events   : " << simulationResult.collision.events.size() << '\n';
    std::cout << "M5 ZMap removed cells : " << simulationResult.zmapRemoval.removedCells << '\n';
    std::cout << "M5 Dexel removed cells: " << simulationResult.dexelRemoval.removedCells << '\n';
    std::cout << "M5 report generated   : " << (simulationResult.report.generated ? "yes" : "no") << '\n';
    std::cout << "M6 Generic generated : " << (genericProgram.generated ? "yes" : "no") << '\n';
    std::cout << "M6 Generic blocks    : " << genericProgram.blocks.size() << '\n';
    std::cout << "M6 FANUC generated   : " << (fanucProgram.generated ? "yes" : "no") << '\n';
    std::cout << "M6 FANUC blocks      : " << fanucProgram.blocks.size() << '\n';
    std::cout << "M6 NC exported       : " << (exportedNc ? "yes" : "no") << '\n';
    std::cout << "M6 NC path           : " << ncOutputPath.string() << '\n';
    std::cout << "M6 Operation Sheet   : " << (operationSheet.generated ? "yes" : "no") << '\n';
    std::cout << "M6 Sheet lines       : " << operationSheet.lines.size() << '\n';
    std::cout << "M7 STEP imported     : " << (stepResult.success ? "yes" : "no") << '\n';
    std::cout << "M7 BRep faces        : " << (activeBrep != nullptr ? activeBrep->GetFaces().size() : 0U) << '\n';
    std::cout << "M7 BRep edges        : " << (activeBrep != nullptr ? activeBrep->GetEdges().size() : 0U) << '\n';
    std::cout << "M7 BRep ray query    : " << (brepRayOk ? "yes" : "no") << '\n';
    std::cout << "M7 BRep plane section: " << (brepPlaneOk ? "yes" : "no") << " sections=" << brepSectionCount << '\n';
    std::cout << "M7 BRep height query : " << (brepHeightOk ? "yes" : "no") << " z=" << brepHeightZ << '\n';
    std::cout << "M7 Face selection    : " << (brepFaceSelectionOk ? "yes" : "no") << '\n';
    std::cout << "M7 Edge selection    : " << (brepEdgeSelectionOk ? "yes" : "no") << '\n';
    std::cout << "M7 Contour moves     : " << brepContourResult.document.moves.size() << '\n';
    std::cout << "M7 Pocket moves      : " << brepPocketResult.document.moves.size() << '\n';
    std::cout << "M7 Drilling moves    : " << brepDrillingResult.document.moves.size() << '\n';
    std::cout << "Validation            : " << (ok ? "PASSED" : "FAILED") << '\n';
    return ok ? 0 : 1;
}
