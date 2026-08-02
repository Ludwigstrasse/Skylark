#pragma once

#include "App/CommandRegistry.h"
#include "App/ProjectSession.h"

#include <QMainWindow>

class QTabWidget;
class QTextEdit;
class QTreeWidget;

namespace skylark::cam
{
class SkylarkViewportWidget;

class MainWindow final : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void BuildMenuBar();
    void BuildRibbonToolBar();
    void BuildCentralLayout();
    void PopulateProjectTree();
    void PopulateOperationPanel();
    void PopulateBottomTabs();
    void RefreshAllViews();
    void CreateNewCamProject();
    void OpenProjectJsonFromDialog();
    void SaveProjectJsonFromDialog();
    void CreateManufacturingSetup();
    void SetTopWcs();
    void DefineBoxStock();
    void DefineFixtureStub();
    void AddFlatEndMillTool();
    void ImportStlFromDialog();
    void ImportStepFromDialog();
    void CreateSyntheticM7BrepModel();
    void CreateSynthetic20MPressureAsset();
    void DrainAccelerationBuilds();
    void PumpGpuUploadQueue();
    void CalculateZLevelToolpath();
    void CalculateParallelFinishingToolpath();
    void CalculateM7BrepContourToolpath();
    void CalculateM7BrepPocketToolpath();
    void CalculateM7BrepDrillingToolpath();
    void SelectDefaultBrepFaceEdge();
    void RunM5Simulation();
    void GenerateGenericIsoNcPreview();
    void GenerateFanucNcPreview();
    void ExportNcFileFromDialog();
    void GenerateOperationSheet();
    void AppendLog(const QString& message);
    void AppendSessionLogTail(std::size_t oldCount);
    void BindCommandHandlers();

    ProjectSession m_session;
    CommandRegistry m_commands;
    QTreeWidget* m_projectTree{nullptr};
    QTreeWidget* m_operationPanel{nullptr};
    QTabWidget* m_bottomTabs{nullptr};
    QTextEdit* m_logView{nullptr};
    QTextEdit* m_warningView{nullptr};
    QTextEdit* m_benchmarkView{nullptr};
    QTextEdit* m_simulationReportView{nullptr};
    QTextEdit* m_ncPreviewView{nullptr};
    QTextEdit* m_operationSheetView{nullptr};
    SkylarkViewportWidget* m_viewport{nullptr};
};
}
