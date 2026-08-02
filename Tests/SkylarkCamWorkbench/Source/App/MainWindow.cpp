#include "App/MainWindow.h"

#include "Diagnostics/BenchmarkReport.h"
#include "LargeMesh/LargeMesh.h"
#include "Render/SkylarkViewportWidget.h"
#include "Simulation/SimulationStubs.h"
#include "Post/PostProcessor.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeWidget>
#include <QStringList>
#include <filesystem>
#include <utility>

namespace skylark::cam
{
namespace
{
QTreeWidgetItem* AddItem(QTreeWidget* parent, const QString& text)
{
    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, text);
    return item;
}

QTreeWidgetItem* AddItem(QTreeWidgetItem* parent, const QString& text)
{
    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, text);
    return item;
}

QString FormatBoundsText(const BoundingBox3d& box)
{
    if (!box.IsValid()) { return QStringLiteral("Bounds: invalid"); }
    const Vec3d size = box.Size();
    return QStringLiteral("Bounds Size: %1 x %2 x %3")
        .arg(size.x, 0, 'f', 3)
        .arg(size.y, 0, 'f', 3)
        .arg(size.z, 0, 'f', 3);
}
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_commands(CreateDefaultCommandRegistry())
{
    setWindowTitle(QString::fromUtf8("SkylarkCamWorkbench M7 - BRep CAM Channel"));
    resize(1600, 980);
    BindCommandHandlers();
    BuildMenuBar();
    BuildRibbonToolBar();
    BuildCentralLayout();
    statusBar()->showMessage(QString::fromUtf8("M7 ready: STEP import, BRep CAM provider, contour/pocket/drilling and diagnostics."));
}

void MainWindow::BindCommandHandlers()
{
    CommandRegistry bound;
    for (const auto& command : m_commands.GetCommands())
    {
        if (command.id == "file.newProject")
        {
            bound.RegisterCommand(command, [this] { CreateNewCamProject(); });
        }
        else if (command.id == "file.openProject")
        {
            bound.RegisterCommand(command, [this] { OpenProjectJsonFromDialog(); });
        }
        else if (command.id == "file.saveProject")
        {
            bound.RegisterCommand(command, [this] { SaveProjectJsonFromDialog(); });
        }
        else if (command.id == "file.importStep")
        {
            bound.RegisterCommand(command, [this] { ImportStepFromDialog(); });
        }
        else if (command.id == "file.syntheticBrep")
        {
            bound.RegisterCommand(command, [this] { CreateSyntheticM7BrepModel(); });
        }
        else if (command.id == "file.importStl")
        {
            bound.RegisterCommand(command, [this] { ImportStlFromDialog(); });
        }
        else if (command.id == "file.synthetic20m")
        {
            bound.RegisterCommand(command, [this] { CreateSynthetic20MPressureAsset(); });
        }
        else if (command.id == "setup.create")
        {
            bound.RegisterCommand(command, [this] { CreateManufacturingSetup(); });
        }
        else if (command.id == "setup.wcs")
        {
            bound.RegisterCommand(command, [this] { SetTopWcs(); });
        }
        else if (command.id == "setup.stock")
        {
            bound.RegisterCommand(command, [this] { DefineBoxStock(); });
        }
        else if (command.id == "setup.fixture")
        {
            bound.RegisterCommand(command, [this] { DefineFixtureStub(); });
        }
        else if (command.id == "setup.addTool")
        {
            bound.RegisterCommand(command, [this] { AddFlatEndMillTool(); });
        }
        else if (command.id == "toolpath.zlevel")
        {
            bound.RegisterCommand(command, [this] { CalculateZLevelToolpath(); });
        }
        else if (command.id == "toolpath.parallel")
        {
            bound.RegisterCommand(command, [this] { CalculateParallelFinishingToolpath(); });
        }
        else if (command.id == "toolpath.contour2d")
        {
            bound.RegisterCommand(command, [this] { CalculateM7BrepContourToolpath(); });
        }
        else if (command.id == "toolpath.pocket2d")
        {
            bound.RegisterCommand(command, [this] { CalculateM7BrepPocketToolpath(); });
        }
        else if (command.id == "toolpath.drilling")
        {
            bound.RegisterCommand(command, [this] { CalculateM7BrepDrillingToolpath(); });
        }
        else if (command.id == "model.brepSelect")
        {
            bound.RegisterCommand(command, [this] { SelectDefaultBrepFaceEdge(); });
        }
        else if (command.id == "model.buildBvh" || command.id == "model.buildLod")
        {
            bound.RegisterCommand(command, [this] { DrainAccelerationBuilds(); });
        }
        else if (command.id == "model.gpuUpload")
        {
            bound.RegisterCommand(command, [this] { PumpGpuUploadQueue(); });
        }
        else if (command.id == "simulation.play" || command.id == "simulation.collision" || command.id == "simulation.stock" || command.id == "simulation.report")
        {
            bound.RegisterCommand(command, [this] { RunM5Simulation(); });
        }
        else if (command.id == "machine.post" || command.id == "machine.ncPreview")
        {
            bound.RegisterCommand(command, [this] { GenerateGenericIsoNcPreview(); });
        }
        else if (command.id == "machine.fanucPreview")
        {
            bound.RegisterCommand(command, [this] { GenerateFanucNcPreview(); });
        }
        else if (command.id == "machine.exportGcode")
        {
            bound.RegisterCommand(command, [this] { ExportNcFileFromDialog(); });
        }
        else if (command.id == "machine.operationSheet")
        {
            bound.RegisterCommand(command, [this] { GenerateOperationSheet(); });
        }
        else if (command.id == "view.fit")
        {
            bound.RegisterCommand(command, [this] { if (m_viewport != nullptr) { m_viewport->FitAll(); } });
        }
        else
        {
            bound.RegisterCommand(command, [this, title = command.title] {
                AppendLog(QString::fromStdString(title + " selected. M7 keeps the CAM command surface alive; implementation belongs to later milestones."));
            });
        }
    }
    m_commands = std::move(bound);
}

void MainWindow::BuildMenuBar()
{
    const QStringList tabs = {"File", "Model", "Setup", "Toolpath", "Simulation", "Machine", "View"};
    for (const QString& tabName : tabs)
    {
        QMenu* menu = menuBar()->addMenu(tabName);
        QString currentGroup;
        for (const auto& command : m_commands.GetCommandsForTab(tabName.toStdString()))
        {
            const QString group = QString::fromStdString(command.group);
            if (!currentGroup.isEmpty() && currentGroup != group) { menu->addSeparator(); }
            currentGroup = group;
            QAction* action = menu->addAction(QString::fromStdString(command.title));
            connect(action, &QAction::triggered, this, [this, id = command.id] { m_commands.Execute(id); });
        }
    }
}

void MainWindow::BuildRibbonToolBar()
{
    QToolBar* ribbon = addToolBar(QString::fromUtf8("CAM Ribbon"));
    ribbon->setMovable(false);
    ribbon->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    const QStringList ids = {"file.newProject", "file.openProject", "file.saveProject", "file.importStep", "file.syntheticBrep", "file.importStl", "file.synthetic20m", "setup.create", "setup.wcs", "setup.stock", "setup.fixture", "setup.addTool", "model.buildBvh", "model.buildLod", "model.gpuUpload", "toolpath.roughing", "toolpath.zlevel", "toolpath.parallel", "toolpath.contour2d", "toolpath.pocket2d", "toolpath.drilling", "model.brepSelect", "simulation.play", "simulation.collision", "simulation.stock", "simulation.report", "machine.ncPreview", "machine.fanucPreview", "machine.exportGcode", "machine.operationSheet", "view.fit"};
    for (const QString& id : ids)
    {
        for (const auto& command : m_commands.GetCommands())
        {
            if (QString::fromStdString(command.id) == id)
            {
                QAction* action = ribbon->addAction(QString::fromStdString(command.title));
                connect(action, &QAction::triggered, this, [this, commandId = command.id] { m_commands.Execute(commandId); });
                break;
            }
        }
    }
}

void MainWindow::BuildCentralLayout()
{
    auto* vertical = new QSplitter(Qt::Vertical, this);
    auto* mainArea = new QSplitter(Qt::Horizontal, vertical);

    m_projectTree = new QTreeWidget(mainArea);
    m_projectTree->setHeaderLabel(QStringLiteral("CAM Project"));
    m_projectTree->setMinimumWidth(340);

    m_viewport = new SkylarkViewportWidget(m_session, mainArea);

    m_operationPanel = new QTreeWidget(mainArea);
    m_operationPanel->setHeaderLabel(QStringLiteral("Operation Parameters"));
    m_operationPanel->setMinimumWidth(340);

    mainArea->setStretchFactor(0, 0);
    mainArea->setStretchFactor(1, 1);
    mainArea->setStretchFactor(2, 0);

    m_bottomTabs = new QTabWidget(vertical);
    m_logView = new QTextEdit(m_bottomTabs);
    m_logView->setReadOnly(true);
    m_warningView = new QTextEdit(m_bottomTabs);
    m_warningView->setReadOnly(true);
    m_benchmarkView = new QTextEdit(m_bottomTabs);
    m_benchmarkView->setReadOnly(true);
    m_simulationReportView = new QTextEdit(m_bottomTabs);
    m_simulationReportView->setReadOnly(true);
    m_ncPreviewView = new QTextEdit(m_bottomTabs);
    m_ncPreviewView->setReadOnly(true);
    m_ncPreviewView->setPlainText(QStringLiteral("NC Preview is ready. Use Machine / NC Preview, FANUC Preview, or Export G-code."));
    m_operationSheetView = new QTextEdit(m_bottomTabs);
    m_operationSheetView->setReadOnly(true);
    m_operationSheetView->setPlainText(QStringLiteral("Operation Sheet will be generated from setup, tools, operations and posted NC program."));
    auto* timelineView = new QTextEdit(m_bottomTabs);
    timelineView->setReadOnly(true);
    timelineView->setPlainText(QStringLiteral("Toolpath timeline: mesh roughing / z-level / parallel plus BRep contour / pocket / drilling; M7 includes STEP/BRep CAM channel."));

    m_bottomTabs->addTab(m_logView, QStringLiteral("Log"));
    m_bottomTabs->addTab(m_warningView, QStringLiteral("Warnings"));
    m_bottomTabs->addTab(m_ncPreviewView, QStringLiteral("NC Code"));
    m_bottomTabs->addTab(m_operationSheetView, QStringLiteral("Operation Sheet"));
    m_bottomTabs->addTab(timelineView, QStringLiteral("Toolpath Timeline"));
    m_bottomTabs->addTab(m_simulationReportView, QStringLiteral("Simulation Report"));
    m_bottomTabs->addTab(m_benchmarkView, QStringLiteral("Benchmark"));

    vertical->setStretchFactor(0, 1);
    vertical->setStretchFactor(1, 0);
    setCentralWidget(vertical);

    RefreshAllViews();
}

void MainWindow::PopulateProjectTree()
{
    if (m_projectTree == nullptr) { return; }
    m_projectTree->clear();
    const CamProject& project = m_session.GetProject();
    QTreeWidgetItem* root = AddItem(m_projectTree, QString::fromStdString(project.GetName()));

    QTreeWidgetItem* models = AddItem(root, QStringLiteral("Imported Models"));
    for (const ImportedModel& model : project.GetImportedModels())
    {
        QTreeWidgetItem* item = AddItem(models, QString::fromStdString(model.name));
        AddItem(item, QStringLiteral("Kind: %1").arg(QString::fromStdString(ToString(model.geometryKind))));
        if (model.geometryKind == CamGeometryKind::Mesh)
        {
            AddItem(item, QStringLiteral("Triangles: %1").arg(static_cast<qulonglong>(model.meshPart.triangleCount)));
            AddItem(item, QStringLiteral("Mesh Chunks: %1").arg(model.meshPart.chunkCount));
            AddItem(item, QStringLiteral("BVH: %1").arg(model.meshPart.bvhReady ? "Ready" : "Queued/Running"));
            AddItem(item, QStringLiteral("BVH Nodes: %1").arg(model.meshPart.bvhNodeCount));
            AddItem(item, QStringLiteral("LOD: %1").arg(model.meshPart.lodReady ? "Ready" : "Queued/Running"));
            AddItem(item, QStringLiteral("LOD Levels: %1").arg(model.meshPart.lodLevelCount));
            AddItem(item, FormatBoundsText(model.meshPart.bounds));
        }
        else if (model.geometryKind == CamGeometryKind::BRep)
        {
            AddItem(item, QStringLiteral("Bodies: %1").arg(model.brepPart.bodyCount));
            AddItem(item, QStringLiteral("Faces: %1").arg(model.brepPart.faceCount));
            AddItem(item, QStringLiteral("Edges: %1").arg(model.brepPart.edgeCount));
            AddItem(item, FormatBoundsText(model.brepPart.bounds));
            AddItem(item, QStringLiteral("Source Entity Map: %1 entries").arg(static_cast<int>(model.sourceEntityMap.size())));
        }
    }

    QTreeWidgetItem* setups = AddItem(root, QStringLiteral("Setups"));
    for (const CamSetup& setup : project.GetSetups())
    {
        QTreeWidgetItem* item = AddItem(setups, QString::fromStdString(setup.name));
        AddItem(item, QString::fromStdString("WCS: " + setup.wcs.name));
        AddItem(item, QString::fromStdString("Stock: " + setup.stock.name));
        AddItem(item, QString::fromStdString("Fixture: " + setup.fixture.name));
        AddItem(item, QString::fromStdString("Machine: " + setup.machine.name));
    }

    QTreeWidgetItem* tools = AddItem(root, QStringLiteral("Tools"));
    for (const ToolDefinition& tool : project.GetToolLibrary().GetTools())
    {
        AddItem(tools, QStringLiteral("T%1 %2 D%3")
            .arg(static_cast<qulonglong>(tool.id), 2, 10, QLatin1Char('0'))
            .arg(QString::fromStdString(tool.name))
            .arg(tool.diameter, 0, 'f', 2));
    }

    QTreeWidgetItem* operations = AddItem(root, QStringLiteral("Operations"));
    for (const OperationNode& operation : project.GetOperationTree().GetOperations())
    {
        AddItem(operations, QStringLiteral("%1 - %2")
            .arg(QString::fromStdString(operation.name))
            .arg(QString::fromStdString(ToString(operation.state))));
    }

    QTreeWidgetItem* simulation = AddItem(root, QStringLiteral("Simulation"));
    if (m_session.HasSimulationResult())
    {
        const SimulationResult& result = m_session.GetLastSimulationResult();
        AddItem(simulation, QStringLiteral("Toolpath Playback: move %1 / %2")
            .arg(static_cast<qulonglong>(result.playback.currentMoveIndex))
            .arg(static_cast<qulonglong>(result.playback.totalMoves)));
        AddItem(simulation, QStringLiteral("Tool Entity: %1").arg(QString::fromStdString(result.toolAssembly.cutter.name)));
        AddItem(simulation, QStringLiteral("Holder Diameter: %1").arg(result.toolAssembly.holderDiameter, 0, 'f', 2));
        AddItem(simulation, QStringLiteral("Collision Events: %1").arg(static_cast<int>(result.collision.events.size())));
        AddItem(simulation, QStringLiteral("ZMap Removed Cells: %1").arg(static_cast<qulonglong>(result.zmapRemoval.removedCells)));
        AddItem(simulation, QStringLiteral("Dexel Removed Cells: %1").arg(static_cast<qulonglong>(result.dexelRemoval.removedCells)));
    }
    else
    {
        AddItem(simulation, QStringLiteral("Toolpath Playback: not run"));
        AddItem(simulation, QStringLiteral("Cutter / Holder Display: waiting for simulation"));
        AddItem(simulation, QStringLiteral("Collision Report: not generated"));
        AddItem(simulation, QStringLiteral("ZMap / Dexel Stock Removal: not generated"));
    }

    QTreeWidgetItem* nc = AddItem(root, QStringLiteral("NC Programs"));
    if (m_session.HasGCodeProgram())
    {
        const GCodeProgram& program = m_session.GetLastGCodeProgram();
        AddItem(nc, QStringLiteral("%1 - %2 blocks").arg(QString::fromStdString(program.programName)).arg(static_cast<int>(program.blocks.size())));
        AddItem(nc, QStringLiteral("Post: %1").arg(QString::fromStdString(program.postName)));
        AddItem(nc, QStringLiteral("Export Extension: .nc"));
    }
    else
    {
        AddItem(nc, QStringLiteral("No NC program generated - use Machine / NC Preview"));
    }
    m_projectTree->expandAll();
}

void MainWindow::PopulateOperationPanel()
{
    if (m_operationPanel == nullptr) { return; }
    m_operationPanel->clear();
    QTreeWidgetItem* op = AddItem(m_operationPanel, QStringLiteral("Operation: Parallel Finishing"));
    QTreeWidgetItem* geometry = AddItem(op, QStringLiteral("Geometry"));
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    AddItem(geometry, asset != nullptr ? QStringLiteral("Drive Model: %1").arg(QString::fromStdString(asset->GetName())) : QStringLiteral("Drive Model: not imported"));
    AddItem(geometry, QStringLiteral("Boundary: Auto / Manual"));
    AddItem(geometry, QStringLiteral("Avoid Regions"));

    QTreeWidgetItem* tool = AddItem(op, QStringLiteral("Tool"));
    AddItem(tool, QStringLiteral("Tool: T02 Ball End Mill D6"));
    AddItem(tool, QStringLiteral("Holder"));
    AddItem(tool, QStringLiteral("Gauge Length"));

    QTreeWidgetItem* heights = AddItem(op, QStringLiteral("Heights"));
    AddItem(heights, QStringLiteral("Clearance"));
    AddItem(heights, QStringLiteral("Retract"));
    AddItem(heights, QStringLiteral("Feed Plane"));
    AddItem(heights, QStringLiteral("Top"));
    AddItem(heights, QStringLiteral("Bottom"));

    QTreeWidgetItem* cutting = AddItem(op, QStringLiteral("Cutting"));
    AddItem(cutting, QStringLiteral("Direction: X / Y / Angle"));
    AddItem(cutting, QStringLiteral("Stepover"));
    AddItem(cutting, QStringLiteral("Tolerance"));
    AddItem(cutting, QStringLiteral("Smoothing"));
    AddItem(cutting, QStringLiteral("Stock To Leave"));

    QTreeWidgetItem* linking = AddItem(op, QStringLiteral("Linking"));
    AddItem(linking, QStringLiteral("Lead In"));
    AddItem(linking, QStringLiteral("Lead Out"));
    AddItem(linking, QStringLiteral("Retract Mode"));
    AddItem(linking, QStringLiteral("Safe Linking"));

    QTreeWidgetItem* manufacturing = AddItem(op, QStringLiteral("M5 Mesh CAM + Simulation State"));
    const CamProject& project = m_session.GetProject();
    AddItem(manufacturing, QStringLiteral("Setups: %1").arg(static_cast<int>(project.GetSetups().size())));
    AddItem(manufacturing, QStringLiteral("Tools: %1").arg(static_cast<int>(project.GetToolLibrary().GetTools().size())));
    if (!project.GetSetups().empty())
    {
        const CamSetup& setup = project.GetSetups().back();
        AddItem(manufacturing, QStringLiteral("Active Setup: %1").arg(QString::fromStdString(setup.name)));
        AddItem(manufacturing, QStringLiteral("WCS: %1 @ (%2, %3, %4)")
            .arg(QString::fromStdString(setup.wcs.name))
            .arg(setup.wcs.origin.x, 0, 'f', 3)
            .arg(setup.wcs.origin.y, 0, 'f', 3)
            .arg(setup.wcs.origin.z, 0, 'f', 3));
        AddItem(manufacturing, QStringLiteral("Box Stock: %1 x %2 x %3")
            .arg(setup.stock.sizeX, 0, 'f', 3)
            .arg(setup.stock.sizeY, 0, 'f', 3)
            .arg(setup.stock.sizeZ, 0, 'f', 3));
        AddItem(manufacturing, QStringLiteral("Fixture: %1").arg(QString::fromStdString(setup.fixture.name)));
    }

    QTreeWidgetItem* diagnostics = AddItem(op, QStringLiteral("M2 LargeMesh Diagnostics"));
    if (asset != nullptr)
    {
        const LargeMeshDiagnostics d = InspectLargeMeshAsset(*asset);
        AddItem(diagnostics, QStringLiteral("Triangles: %1").arg(static_cast<qulonglong>(d.triangleCount)));
        AddItem(diagnostics, QStringLiteral("Chunks: %1").arg(d.chunkCount));
        AddItem(diagnostics, QStringLiteral("Preview Triangles: %1").arg(d.previewTriangleCount));
        AddItem(diagnostics, QStringLiteral("Preview Stride: %1").arg(d.previewStride));
        AddItem(diagnostics, QStringLiteral("File Size: %1").arg(QString::fromStdString(FormatByteSize(d.fileSizeBytes))));
        AddItem(diagnostics, QStringLiteral("Estimated GPU Payload: %1").arg(QString::fromStdString(FormatByteSize(d.estimatedMemoryBytes))));
        AddItem(diagnostics, QStringLiteral("Import Time: %1 s").arg(d.importTimeSeconds, 0, 'f', 3));
        AddItem(diagnostics, QStringLiteral("BVH State: %1").arg(QString::fromStdString(ToString(d.bvhState))));
        AddItem(diagnostics, QStringLiteral("BVH Nodes: %1").arg(d.bvhNodeCount));
        AddItem(diagnostics, QStringLiteral("LOD State: %1").arg(QString::fromStdString(ToString(d.lodState))));
        AddItem(diagnostics, QStringLiteral("LOD Levels: %1").arg(d.lodLevelCount));
    }

    QTreeWidgetItem* simulation = AddItem(op, QStringLiteral("M5 Simulation"));
    if (m_session.HasSimulationResult())
    {
        const SimulationResult& result = m_session.GetLastSimulationResult();
        AddItem(simulation, QStringLiteral("Playback: move %1 / %2")
            .arg(static_cast<qulonglong>(result.playback.currentMoveIndex))
            .arg(static_cast<qulonglong>(result.playback.totalMoves)));
        AddItem(simulation, QStringLiteral("Tool: %1 D%2")
            .arg(QString::fromStdString(result.toolAssembly.cutter.name))
            .arg(result.toolAssembly.cutter.diameter, 0, 'f', 2));
        AddItem(simulation, QStringLiteral("Holder: D%1 L%2")
            .arg(result.toolAssembly.holderDiameter, 0, 'f', 2)
            .arg(result.toolAssembly.holderLength, 0, 'f', 2));
        AddItem(simulation, QStringLiteral("Collision Events: %1").arg(static_cast<int>(result.collision.events.size())));
        AddItem(simulation, QStringLiteral("ZMap Removed Cells: %1").arg(static_cast<qulonglong>(result.zmapRemoval.removedCells)));
        AddItem(simulation, QStringLiteral("Dexel Removed Cells: %1").arg(static_cast<qulonglong>(result.dexelRemoval.removedCells)));
    }
    else
    {
        AddItem(simulation, QStringLiteral("Run Simulation / Play to generate playback, cutter, holder, stock removal and report."));
    }

    QTreeWidgetItem* post = AddItem(op, QStringLiteral("M6 Post Processing"));
    if (m_session.HasGCodeProgram())
    {
        const GCodeProgram& program = m_session.GetLastGCodeProgram();
        AddItem(post, QStringLiteral("Post: %1").arg(QString::fromStdString(program.postName)));
        AddItem(post, QStringLiteral("Program: %1").arg(QString::fromStdString(program.programName)));
        AddItem(post, QStringLiteral("NC Blocks: %1").arg(static_cast<int>(program.blocks.size())));
        AddItem(post, QStringLiteral("Warnings: %1").arg(static_cast<int>(program.warnings.size())));
    }
    else
    {
        AddItem(post, QStringLiteral("NC Preview: not generated"));
        AddItem(post, QStringLiteral("Export .nc: waiting for post"));
    }
    if (m_session.HasOperationSheet())
    {
        AddItem(post, QStringLiteral("Operation Sheet: generated"));
    }
    else
    {
        AddItem(post, QStringLiteral("Operation Sheet: not generated"));
    }

    QTreeWidgetItem* brep = AddItem(op, QStringLiteral("M7 BRep CAM Channel"));
    const BrepModel* activeBrep = m_session.GetActiveBrepModel();
    if (activeBrep != nullptr)
    {
        AddItem(brep, QStringLiteral("Active BRep: %1").arg(QString::fromStdString(activeBrep->GetName())));
        AddItem(brep, QStringLiteral("Faces: %1").arg(static_cast<int>(activeBrep->GetFaces().size())));
        AddItem(brep, QStringLiteral("Edges: %1").arg(static_cast<int>(activeBrep->GetEdges().size())));
        AddItem(brep, QStringLiteral("Holes: %1").arg(static_cast<int>(activeBrep->GetHoles().size())));
        AddItem(brep, QStringLiteral("Pockets: %1").arg(static_cast<int>(activeBrep->GetPockets().size())));
        AddItem(brep, QStringLiteral("Selected Faces: %1").arg(static_cast<int>(m_session.GetBrepSelection().faceIds.size())));
        AddItem(brep, QStringLiteral("Selected Edges: %1").arg(static_cast<int>(m_session.GetBrepSelection().edgeIds.size())));
    }
    else
    {
        AddItem(brep, QStringLiteral("STEP/BRep model: not imported"));
        AddItem(brep, QStringLiteral("Use File / Import STEP or Synthetic BRep"));
    }
    if (m_session.HasBrepDiagnostics())
    {
        const BrepFeatureDiagnostics& d = m_session.GetLastBrepDiagnostics();
        AddItem(brep, QString::fromStdString(d.summary));
        AddItem(brep, QStringLiteral("Warnings: %1").arg(static_cast<int>(d.warnings.size())));
    }

    QTreeWidgetItem* render = AddItem(op, QStringLiteral("Skylark RenderProxy / GPU Queue"));
    const auto& proxies = m_session.GetLargeMeshRenderProxies();
    AddItem(render, QStringLiteral("LargeMeshRenderProxy Count: %1").arg(static_cast<int>(proxies.size())));
    if (!proxies.empty())
    {
        AddItem(render, QStringLiteral("Active Proxy Chunks: %1").arg(static_cast<int>(proxies.back().chunks.size())));
    }
    const GpuUploadQueueSnapshot upload = m_session.GetGpuUploadQueueSnapshot();
    AddItem(render, QStringLiteral("GPU Requests: %1").arg(static_cast<qulonglong>(upload.totalRequests)));
    AddItem(render, QStringLiteral("GPU Pending: %1 / %2").arg(static_cast<qulonglong>(upload.pendingRequests)).arg(QString::fromStdString(FormatByteSize(upload.pendingBytes))));
    AddItem(render, QStringLiteral("GPU Uploaded: %1 / %2").arg(static_cast<qulonglong>(upload.uploadedRequests)).arg(QString::fromStdString(FormatByteSize(upload.uploadedBytes))));
    AddItem(render, QStringLiteral("Background Builds Pending: %1").arg(m_session.GetPendingAccelerationBuildCount()));

    m_operationPanel->expandAll();
}

void MainWindow::PopulateBottomTabs()
{
    if (m_logView == nullptr || m_warningView == nullptr || m_benchmarkView == nullptr || m_simulationReportView == nullptr || m_ncPreviewView == nullptr || m_operationSheetView == nullptr) { return; }
    m_logView->clear();
    for (const std::string& message : m_session.GetLogMessages())
    {
        m_logView->append(QString::fromStdString(message));
    }

    m_warningView->clear();
    m_warningView->append(QStringLiteral("M7 architecture constraints:"));
    m_warningView->append(QStringLiteral("- STL is not converted to TopoDS_Shape or BRep."));
    m_warningView->append(QStringLiteral("- Post processing consumes ToolpathDocument + ToolLibrary + Setup; it does not read STL files directly."));
    m_warningView->append(QStringLiteral("- Cutter/holder are manufacturing simulation objects, not mesh import objects."));
    m_warningView->append(QStringLiteral("- Generic ISO and FANUC are first-version 3-axis posts; machine-specific post customization belongs to later milestones."));
    m_warningView->append(QStringLiteral("- STEP/BRep CAM uses BRepCamGeometryProvider; UI does not build toolpaths directly."));
    m_warningView->append(QStringLiteral("- M7 StepBrepImporter is a topology proxy entry; exact OCCT STEP topology can replace it behind the same provider boundary."));

    m_simulationReportView->clear();
    if (m_session.HasSimulationResult())
    {
        m_simulationReportView->setPlainText(QString::fromStdString(m_session.GetLastSimulationResult().report.text));
    }
    else
    {
        m_simulationReportView->setPlainText(QStringLiteral("Simulation report has not been generated. Run Simulation / Play."));
    }

    m_ncPreviewView->clear();
    if (m_session.HasGCodeProgram())
    {
        m_ncPreviewView->setPlainText(QString::fromStdString(GenerateNcPreview(m_session.GetLastGCodeProgram(), 300U)));
    }
    else
    {
        m_ncPreviewView->setPlainText(QStringLiteral("No NC program generated. Use Machine / NC Preview or Export G-code."));
    }

    m_operationSheetView->clear();
    if (m_session.HasOperationSheet())
    {
        m_operationSheetView->setPlainText(QString::fromStdString(m_session.GetLastOperationSheet().text));
    }
    else
    {
        m_operationSheetView->setPlainText(QStringLiteral("No operation sheet generated. Use Machine / Operation Sheet."));
    }

    m_session.PollLargeMeshAccelerationBuilds();
    BenchmarkReport report = CreateM2BenchmarkReport(m_session.GetActiveLargeMeshAsset(), m_session.GetGpuUploadQueueSnapshot(), m_session.GetPendingAccelerationBuildCount());
    m_benchmarkView->clear();
    for (const BenchmarkCounter& counter : report.counters)
    {
        m_benchmarkView->append(QString::fromStdString(counter.name + ": " + counter.value));
    }
}

void MainWindow::RefreshAllViews()
{
    PopulateProjectTree();
    PopulateOperationPanel();
    PopulateBottomTabs();
    if (m_viewport != nullptr)
    {
        m_viewport->FitAll();
        m_viewport->update();
    }
}


void MainWindow::CreateNewCamProject()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    m_session.CreateNewCamProject("Skylark CAM Project - M7 User Project");
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Created new M7 CAM project"));
    RefreshAllViews();
}

void MainWindow::OpenProjectJsonFromDialog()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open CAM Project JSON"),
        QString(),
        QStringLiteral("Skylark CAM Project (*.skcam.json *.json);;All Files (*.*)"));
    if (fileName.isEmpty()) { return; }

    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const bool ok = m_session.LoadProjectJson(std::filesystem::path(fileName.toStdString()));
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(ok ? QStringLiteral("Loaded CAM project JSON") : QStringLiteral("Failed to load CAM project JSON"));
    RefreshAllViews();
}

void MainWindow::SaveProjectJsonFromDialog()
{
    const QString fileName = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("Save CAM Project JSON"),
        QStringLiteral("skylark_cam_project_m6.skcam.json"),
        QStringLiteral("Skylark CAM Project (*.skcam.json *.json);;All Files (*.*)"));
    if (fileName.isEmpty()) { return; }

    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const bool ok = m_session.SaveProjectJson(std::filesystem::path(fileName.toStdString()));
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(ok ? QStringLiteral("Saved CAM project JSON") : QStringLiteral("Failed to save CAM project JSON"));
    RefreshAllViews();
}

void MainWindow::CreateManufacturingSetup()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    m_session.CreateSetup(QStringLiteral("Setup %1 - 3 Axis Mill").arg(static_cast<int>(m_session.GetProject().GetSetups().size() + 1U)).toStdString());
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Created CAM setup"));
    RefreshAllViews();
}

void MainWindow::SetTopWcs()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    WorkCoordinateSystem wcs;
    wcs.name = "G54 Top";
    wcs.origin = {0.0, 0.0, 40.0};
    wcs.xAxis = {1.0, 0.0, 0.0};
    wcs.yAxis = {0.0, 1.0, 0.0};
    wcs.zAxis = {0.0, 0.0, 1.0};
    m_session.SetActiveSetupWcs(wcs);
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Set active setup WCS to G54 Top"));
    RefreshAllViews();
}

void MainWindow::DefineBoxStock()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    StockDefinition stock;
    stock.name = "Box Stock 160 x 100 x 50";
    stock.sizeX = 160.0;
    stock.sizeY = 100.0;
    stock.sizeZ = 50.0;
    stock.offset = 3.0;
    m_session.DefineActiveSetupBoxStock(stock);
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Defined active setup box stock"));
    RefreshAllViews();
}

void MainWindow::DefineFixtureStub()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    FixtureDefinition fixture;
    fixture.name = "Fixture Stub - Modular Vise";
    fixture.collisionEnabled = true;
    m_session.DefineActiveSetupFixtureStub(fixture);
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Defined active setup fixture stub"));
    RefreshAllViews();
}

void MainWindow::AddFlatEndMillTool()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    ToolDefinition tool;
    tool.id = 0;
    tool.name = "Flat End Mill D12 - M3 Added";
    tool.kind = CutterKind::FlatEndMill;
    tool.diameter = 12.0;
    tool.cornerRadius = 0.0;
    tool.fluteLength = 36.0;
    tool.gaugeLength = 72.0;
    m_session.AddToolToLibrary(tool);
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Added tool to active CAM tool library"));
    RefreshAllViews();
}



void MainWindow::ImportStepFromDialog()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Import STEP for BRep CAM"),
        QString(),
        QStringLiteral("STEP Files (*.step *.stp);;All Files (*.*)"));
    if (fileName.isEmpty()) { return; }

    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const StepBrepImportResult result = m_session.ImportStepAsBRep(std::filesystem::path(fileName.toStdString()));
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success
        ? QStringLiteral("M7 STEP imported: %1 faces, %2 edges")
            .arg(result.diagnostics.faceCount)
            .arg(result.diagnostics.edgeCount)
        : QStringLiteral("M7 STEP import failed"));
    RefreshAllViews();
}

void MainWindow::CreateSyntheticM7BrepModel()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const StepBrepImportResult result = m_session.CreateM7SyntheticBRepModel();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("M7 synthetic BRep created: %1 faces, %2 edges")
        .arg(result.diagnostics.faceCount)
        .arg(result.diagnostics.edgeCount));
    RefreshAllViews();
}

void MainWindow::ImportStlFromDialog()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Import Binary STL for CAM LargeMesh"),
        QString(),
        QStringLiteral("STL Files (*.stl);;All Files (*.*)"));
    if (fileName.isEmpty()) { return; }

    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    LargeMeshImportOptions options;
    options.preferredTrianglesPerChunk = 250000U;
    options.maxPreviewTriangles = 300000U;
    const BinaryStlReadResult result = m_session.ImportBinaryStl(std::filesystem::path(fileName.toStdString()), options);
    AppendSessionLogTail(oldLogCount);
    if (result.diagnostics.success)
    {
        statusBar()->showMessage(QStringLiteral("Imported %1 triangles in %2 s")
            .arg(static_cast<qulonglong>(result.diagnostics.importedTriangleCount))
            .arg(result.diagnostics.importTimeSeconds, 0, 'f', 3));
    }
    else
    {
        statusBar()->showMessage(QStringLiteral("Binary STL import failed"));
    }
    RefreshAllViews();
}

void MainWindow::CreateSynthetic20MPressureAsset()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const BinaryStlReadResult result = m_session.CreateSynthetic20MPressureAsset();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("M2 20M pressure entry created: %1 triangles, %2 chunks")
        .arg(static_cast<qulonglong>(result.diagnostics.importedTriangleCount))
        .arg(static_cast<int>(result.asset.GetChunks().size())));
    RefreshAllViews();
}

void MainWindow::DrainAccelerationBuilds()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const std::uint32_t completed = m_session.DrainLargeMeshAccelerationBuilds();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("Background BVH/LOD completed: %1 job(s)").arg(completed));
    RefreshAllViews();
}

void MainWindow::PumpGpuUploadQueue()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const std::uint32_t uploaded = m_session.PumpGpuUploads(16U);
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(QStringLiteral("GPU upload queue pumped: %1 chunk request(s)").arg(uploaded));
    RefreshAllViews();
}


void MainWindow::CalculateZLevelToolpath()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const ToolpathResult result = m_session.CalculateZLevelToolpath();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success ? QStringLiteral("M4 Z-Level toolpath calculated") : QStringLiteral("M4 Z-Level toolpath failed"));
    RefreshAllViews();
}

void MainWindow::CalculateParallelFinishingToolpath()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const ToolpathResult result = m_session.CalculateParallelFinishingToolpath();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success ? QStringLiteral("M4 Parallel Finishing toolpath calculated") : QStringLiteral("M4 Parallel Finishing toolpath failed"));
    RefreshAllViews();
}


void MainWindow::CalculateM7BrepContourToolpath()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const ToolpathResult result = m_session.CalculateM7BrepContourToolpath();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success ? QStringLiteral("M7 BRep 2.5D contour calculated") : QStringLiteral("M7 BRep contour failed"));
    RefreshAllViews();
}

void MainWindow::CalculateM7BrepPocketToolpath()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const ToolpathResult result = m_session.CalculateM7BrepPocketToolpath();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success ? QStringLiteral("M7 BRep pocket stub calculated") : QStringLiteral("M7 BRep pocket failed"));
    RefreshAllViews();
}

void MainWindow::CalculateM7BrepDrillingToolpath()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const ToolpathResult result = m_session.CalculateM7BrepDrillingToolpath();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success ? QStringLiteral("M7 BRep drilling stub calculated") : QStringLiteral("M7 BRep drilling failed"));
    RefreshAllViews();
}

void MainWindow::SelectDefaultBrepFaceEdge()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    bool ok = m_session.SelectM7BrepFace(2U);
    ok = m_session.SelectM7BrepEdge(1U) || ok;
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(ok ? QStringLiteral("M7 selected default BRep face/edge") : QStringLiteral("M7 BRep selection failed - import STEP first"));
    RefreshAllViews();
}

void MainWindow::RunM5Simulation()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const SimulationResult result = m_session.RunM5Simulation();
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(result.success
        ? QStringLiteral("M5 simulation report generated: playback, collision, ZMap and Dexel complete")
        : QStringLiteral("M5 simulation report generated with validation warnings"));
    RefreshAllViews();
}

void MainWindow::GenerateGenericIsoNcPreview()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const GCodeProgram program = m_session.GenerateM6GenericIsoProgram();
    AppendSessionLogTail(oldLogCount);
    if (m_ncPreviewView != nullptr)
    {
        m_ncPreviewView->setPlainText(QString::fromStdString(GenerateNcPreview(program, 300U)));
        m_bottomTabs->setCurrentWidget(m_ncPreviewView);
    }
    statusBar()->showMessage(QStringLiteral("M6 Generic ISO NC preview generated: %1 blocks").arg(static_cast<int>(program.blocks.size())));
    RefreshAllViews();
}

void MainWindow::GenerateFanucNcPreview()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const GCodeProgram program = m_session.GenerateM6FanucProgram();
    AppendSessionLogTail(oldLogCount);
    if (m_ncPreviewView != nullptr)
    {
        m_ncPreviewView->setPlainText(QString::fromStdString(GenerateNcPreview(program, 300U)));
        m_bottomTabs->setCurrentWidget(m_ncPreviewView);
    }
    statusBar()->showMessage(QStringLiteral("M6 FANUC 3-axis NC preview generated: %1 blocks").arg(static_cast<int>(program.blocks.size())));
    RefreshAllViews();
}

void MainWindow::ExportNcFileFromDialog()
{
    const QString fileName = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("Export NC Program"),
        QStringLiteral("fanuc_setup1.nc"),
        QStringLiteral("NC Program (*.nc *.tap *.iso);;All Files (*.*)"));
    if (fileName.isEmpty()) { return; }

    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const bool ok = m_session.ExportM6NcFile(std::filesystem::path(fileName.toStdString()));
    AppendSessionLogTail(oldLogCount);
    statusBar()->showMessage(ok ? QStringLiteral("Exported NC program") : QStringLiteral("Failed to export NC program"));
    RefreshAllViews();
}

void MainWindow::GenerateOperationSheet()
{
    const std::size_t oldLogCount = m_session.GetLogMessages().size();
    const OperationSheet sheet = m_session.GenerateM6OperationSheet();
    AppendSessionLogTail(oldLogCount);
    if (m_operationSheetView != nullptr)
    {
        m_operationSheetView->setPlainText(QString::fromStdString(sheet.text));
        m_bottomTabs->setCurrentWidget(m_operationSheetView);
    }
    statusBar()->showMessage(QStringLiteral("M6 operation sheet generated"));
    RefreshAllViews();
}

void MainWindow::AppendLog(const QString& message)
{
    m_session.AppendLog(message.toStdString());
    if (m_logView != nullptr) { m_logView->append(message); }
}

void MainWindow::AppendSessionLogTail(std::size_t oldCount)
{
    if (m_logView == nullptr) { return; }
    const std::vector<std::string>& messages = m_session.GetLogMessages();
    for (std::size_t i = oldCount; i < messages.size(); ++i)
    {
        m_logView->append(QString::fromStdString(messages[i]));
    }
}
}
