#include "ModelIO/SKStlFastReader.h"
#include "Voxel/SKHexahedralMesh.h"
#include "Voxel/SKVolumeVoxelGrid.h"

#include <QAbstractItemView>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDateTime>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLinearGradient>
#include <QLocale>
#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPen>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

namespace
{
    using namespace Skylark::VolumeStudio;

    QString FormatCount(const std::uint64_t Value)
    {
        return QLocale(QLocale::English).toString(static_cast<qulonglong>(Value));
    }

    QString FormatBytes(const std::uint64_t Bytes)
    {
        const double KiB = 1024.0;
        const double MiB = KiB * 1024.0;
        const double GiB = MiB * 1024.0;
        if (Bytes >= static_cast<std::uint64_t>(GiB))
        {
            return QString::number(static_cast<double>(Bytes) / GiB, 'f', 2) + QStringLiteral(" GB");
        }
        if (Bytes >= static_cast<std::uint64_t>(MiB))
        {
            return QString::number(static_cast<double>(Bytes) / MiB, 'f', 2) + QStringLiteral(" MB");
        }
        if (Bytes >= static_cast<std::uint64_t>(KiB))
        {
            return QString::number(static_cast<double>(Bytes) / KiB, 'f', 2) + QStringLiteral(" KB");
        }
        return QString::number(static_cast<qulonglong>(Bytes)) + QStringLiteral(" B");
    }

    class FMetricCard final : public QFrame
    {
    public:
        FMetricCard(const QString& Title, const QString& Value, QWidget* Parent = nullptr)
            : QFrame(Parent)
        {
            setObjectName(QStringLiteral("MetricCard"));
            setFrameShape(QFrame::NoFrame);
            setMinimumHeight(72);

            QVBoxLayout* Layout = new QVBoxLayout(this);
            Layout->setContentsMargins(14, 10, 14, 10);
            Layout->setSpacing(4);

            TitleLabel = new QLabel(Title, this);
            TitleLabel->setObjectName(QStringLiteral("MetricTitle"));
            ValueLabel = new QLabel(Value, this);
            ValueLabel->setObjectName(QStringLiteral("MetricValue"));

            Layout->addWidget(TitleLabel);
            Layout->addWidget(ValueLabel);
        }

        void SetValue(const QString& Value)
        {
            ValueLabel->setText(Value);
        }

    private:
        QLabel* TitleLabel = nullptr;
        QLabel* ValueLabel = nullptr;
    };

    class FStudioViewport final : public QWidget
    {
    public:
        FStudioViewport(QWidget* Parent = nullptr)
            : QWidget(Parent)
        {
            setMinimumSize(740, 460);
            setAutoFillBackground(false);
        }

        void SetModelState(const QString& Name,
                           const std::uint64_t TriangleCount,
                           const FSKVolumeStudioAabb& Bounds,
                           const bool bHasModel)
        {
            ModelName = Name;
            MeshTriangleCount = TriangleCount;
            MeshBounds = Bounds;
            bModelLoaded = bHasModel;
            update();
        }

        void SetVoxelState(const FSKVolumeVoxelGrid& Grid, const FSKHexahedralMesh& HexMesh, const bool bHasVoxel)
        {
            VoxelDimX = Grid.DimX;
            VoxelDimY = Grid.DimY;
            VoxelDimZ = Grid.DimZ;
            HexCellCount = HexMesh.GetCellCount();
            HexNodeCount = HexMesh.GetNodeCount();
            bVoxelReady = bHasVoxel;
            update();
        }

    protected:
        void paintEvent(QPaintEvent*) override
        {
            QPainter Painter(this);
            Painter.setRenderHint(QPainter::Antialiasing, true);
            DrawBackground(Painter);
            DrawGrid(Painter);
            DrawScene(Painter);
            DrawOverlay(Painter);
            DrawAxisGizmo(Painter);
        }

    private:
        QString ModelName = QStringLiteral("No production model loaded");
        FSKVolumeStudioAabb MeshBounds;
        std::uint64_t MeshTriangleCount = 0;
        std::size_t HexCellCount = 0;
        std::size_t HexNodeCount = 0;
        int VoxelDimX = 0;
        int VoxelDimY = 0;
        int VoxelDimZ = 0;
        bool bModelLoaded = false;
        bool bVoxelReady = false;

        void DrawBackground(QPainter& Painter)
        {
            QLinearGradient Gradient(rect().topLeft(), rect().bottomRight());
            Gradient.setColorAt(0.0, QColor(15, 22, 32));
            Gradient.setColorAt(0.55, QColor(23, 33, 47));
            Gradient.setColorAt(1.0, QColor(9, 13, 20));
            Painter.fillRect(rect(), Gradient);
        }

        void DrawGrid(QPainter& Painter)
        {
            Painter.save();
            Painter.setPen(QPen(QColor(62, 74, 91, 120), 1));
            const int Step = 42;
            for (int X = 0; X < width(); X += Step)
            {
                Painter.drawLine(X, 0, X, height());
            }
            for (int Y = 0; Y < height(); Y += Step)
            {
                Painter.drawLine(0, Y, width(), Y);
            }
            Painter.setPen(QPen(QColor(112, 130, 156, 80), 1));
            Painter.drawLine(width() / 2, 0, width() / 2, height());
            Painter.drawLine(0, height() / 2, width(), height() / 2);
            Painter.restore();
        }

        QPointF Project(const float X, const float Y, const float Z, const QRectF& View) const
        {
            const double Scale = std::min(View.width(), View.height()) * 0.26;
            const double IsoX = (static_cast<double>(X) - static_cast<double>(Y)) * 0.78;
            const double IsoY = (static_cast<double>(X) + static_cast<double>(Y)) * 0.34 - static_cast<double>(Z) * 0.78;
            return QPointF(View.center().x() + IsoX * Scale, View.center().y() + IsoY * Scale);
        }

        void DrawScene(QPainter& Painter)
        {
            Painter.save();
            QRectF ViewRect = rect().adjusted(60, 42, -300, -56);
            if (ViewRect.width() < 320.0)
            {
                ViewRect = rect().adjusted(40, 42, -40, -56);
            }

            if (!bModelLoaded)
            {
                Painter.setPen(QPen(QColor(129, 145, 170), 1));
                Painter.setFont(QFont(QStringLiteral("Segoe UI"), 14, QFont::DemiBold));
                Painter.drawText(ViewRect, Qt::AlignCenter, QStringLiteral("Skylark CAM Manufacturing Viewport\nImport a large STL/STEP/OBJ/PLY/glTF/JT/3DXML model to start."));
                Painter.restore();
                return;
            }

            std::array<QPointF, 8> Points = {
                Project(-1.0F, -0.75F, -0.45F, ViewRect),
                Project( 1.0F, -0.75F, -0.45F, ViewRect),
                Project( 1.0F,  0.75F, -0.45F, ViewRect),
                Project(-1.0F,  0.75F, -0.45F, ViewRect),
                Project(-1.0F, -0.75F,  0.45F, ViewRect),
                Project( 1.0F, -0.75F,  0.45F, ViewRect),
                Project( 1.0F,  0.75F,  0.45F, ViewRect),
                Project(-1.0F,  0.75F,  0.45F, ViewRect)
            };

            QPolygonF TopFace;
            TopFace << Points[4] << Points[5] << Points[6] << Points[7];
            QPolygonF FrontFace;
            FrontFace << Points[0] << Points[1] << Points[5] << Points[4];
            QPolygonF RightFace;
            RightFace << Points[1] << Points[2] << Points[6] << Points[5];

            Painter.setPen(Qt::NoPen);
            Painter.setBrush(QColor(60, 140, 220, 70));
            Painter.drawPolygon(FrontFace);
            Painter.setBrush(QColor(45, 190, 145, 58));
            Painter.drawPolygon(RightFace);
            Painter.setBrush(QColor(210, 165, 72, 62));
            Painter.drawPolygon(TopFace);

            Painter.setPen(QPen(QColor(170, 205, 255, 210), 2));
            const std::array<std::pair<int, int>, 12> Edges = {{
                {0, 1}, {1, 2}, {2, 3}, {3, 0},
                {4, 5}, {5, 6}, {6, 7}, {7, 4},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}
            }};
            for (const auto& Edge : Edges)
            {
                Painter.drawLine(Points[Edge.first], Points[Edge.second]);
            }

            if (bVoxelReady)
            {
                Painter.setPen(QPen(QColor(250, 210, 90, 150), 1, Qt::DashLine));
                QRectF VoxelRect = QRectF(ViewRect.center().x() - ViewRect.width() * 0.28,
                                          ViewRect.center().y() - ViewRect.height() * 0.27,
                                          ViewRect.width() * 0.56,
                                          ViewRect.height() * 0.54);
                Painter.drawRect(VoxelRect);
                for (int I = 1; I < 6; ++I)
                {
                    const double X = VoxelRect.left() + VoxelRect.width() * static_cast<double>(I) / 6.0;
                    const double Y = VoxelRect.top() + VoxelRect.height() * static_cast<double>(I) / 6.0;
                    Painter.drawLine(QPointF(X, VoxelRect.top()), QPointF(X, VoxelRect.bottom()));
                    Painter.drawLine(QPointF(VoxelRect.left(), Y), QPointF(VoxelRect.right(), Y));
                }
            }
            Painter.restore();
        }

        void DrawOverlay(QPainter& Painter)
        {
            Painter.save();
            const QRectF Panel(width() - 280, 28, 250, bVoxelReady ? 190 : 145);
            Painter.setPen(QPen(QColor(73, 92, 118), 1));
            Painter.setBrush(QColor(12, 18, 28, 210));
            Painter.drawRoundedRect(Panel, 10, 10);

            Painter.setPen(QColor(218, 228, 242));
            Painter.setFont(QFont(QStringLiteral("Consolas"), 9, QFont::DemiBold));
            int Y = static_cast<int>(Panel.top()) + 28;
            Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("VIEW MODE : Manufacturing"));
            Y += 24;
            Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("BACKEND   : Skylark Preview"));
            Y += 24;
            Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("TRIANGLES : %1").arg(FormatCount(MeshTriangleCount)));
            Y += 24;
            Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("LOD       : Interactive/Full"));
            if (bVoxelReady)
            {
                Y += 24;
                Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("VOXEL     : %1 x %2 x %3").arg(VoxelDimX).arg(VoxelDimY).arg(VoxelDimZ));
                Y += 24;
                Painter.drawText(static_cast<int>(Panel.left()) + 16, Y, QStringLiteral("HEX CELLS : %1").arg(FormatCount(static_cast<std::uint64_t>(HexCellCount))));
            }
            Painter.restore();
        }

        void DrawAxisGizmo(QPainter& Painter)
        {
            Painter.save();
            const QPointF Origin(62, height() - 62);
            Painter.setPen(QPen(QColor(230, 82, 82), 3));
            Painter.drawLine(Origin, Origin + QPointF(54, 0));
            Painter.drawText(Origin + QPointF(62, 4), QStringLiteral("X"));
            Painter.setPen(QPen(QColor(80, 210, 120), 3));
            Painter.drawLine(Origin, Origin + QPointF(-22, -42));
            Painter.drawText(Origin + QPointF(-32, -48), QStringLiteral("Y"));
            Painter.setPen(QPen(QColor(82, 150, 240), 3));
            Painter.drawLine(Origin, Origin + QPointF(0, -62));
            Painter.drawText(Origin + QPointF(-4, -72), QStringLiteral("Z"));
            Painter.restore();
        }
    };

    class FVolumeStudioWindow final : public QMainWindow
    {
    public:
        FVolumeStudioWindow()
        {
            setWindowTitle(QStringLiteral("Skylark Volume Studio - CAM Manufacturing Workbench"));
            resize(1560, 940);
            setMinimumSize(1280, 760);

            CreateActions();
            CreateMenus();
            CreateMainLayout();
            CreateDocks();
            CreateStatusBar();
            ApplyCommercialStyle();
            ResetProjectTree();
            ResetOperationTree();
            UpdateMetricsEmpty();
            AppendLog(QStringLiteral("[SYSTEM] Skylark Volume Studio initialized. Backend bridge: Skylark preview / VTK future / OCCViewer precise CAD future."));
        }

    private:
        QAction* ImportStlAction = nullptr;
        QAction* BuildVoxelAction = nullptr;
        QAction* PackageAction = nullptr;
        QAction* ExitAction = nullptr;

        FStudioViewport* Viewport = nullptr;
        QTextEdit* Console = nullptr;
        QTreeWidget* ProjectTree = nullptr;
        QTreeWidget* OperationTree = nullptr;
        QTableWidget* PropertyTable = nullptr;
        QTableWidget* StatisticsTable = nullptr;
        QTableWidget* ToolTable = nullptr;
        QTableWidget* JobTable = nullptr;
        QProgressBar* ImportProgress = nullptr;
        QLabel* StatusText = nullptr;
        QLabel* BackendText = nullptr;
        QLabel* MemoryBudgetText = nullptr;
        QComboBox* BackendCombo = nullptr;

        FMetricCard* TriangleCard = nullptr;
        FMetricCard* MemoryCard = nullptr;
        FMetricCard* VoxelCard = nullptr;
        FMetricCard* HexCard = nullptr;

        std::unique_ptr<FSKVolumeStudioMeshAsset> CurrentMesh;
        std::unique_ptr<FSKHexahedralMesh> CurrentHexMesh;
        FSKVolumeVoxelGrid CurrentVoxelGrid;
        QString CurrentFileName;
        QString CurrentBaseName;

        void CreateActions()
        {
            ImportStlAction = new QAction(QStringLiteral("Import Binary STL"), this);
            BuildVoxelAction = new QAction(QStringLiteral("Build Hex Voxel Mesh"), this);
            PackageAction = new QAction(QStringLiteral("Package Manufacturing Job"), this);
            ExitAction = new QAction(QStringLiteral("Exit"), this);

            BuildVoxelAction->setEnabled(false);
            PackageAction->setEnabled(false);

            QObject::connect(ImportStlAction, &QAction::triggered, this, [this]() { OpenBinaryStl(); });
            QObject::connect(BuildVoxelAction, &QAction::triggered, this, [this]() { BuildVoxelMesh(); });
            QObject::connect(PackageAction, &QAction::triggered, this, [this]() { AppendLog(QStringLiteral("[PLAN] Manufacturing package pipeline reserved: setup sheet, stock, tool list, NC output.")); });
            QObject::connect(ExitAction, &QAction::triggered, this, [this]() { close(); });
        }

        void CreateMenus()
        {
            QMenu* FileMenu = menuBar()->addMenu(QStringLiteral("File"));
            FileMenu->addAction(ImportStlAction);
            FileMenu->addSeparator();
            FileMenu->addAction(ExitAction);

            QMenu* ExchangeMenu = menuBar()->addMenu(QStringLiteral("Exchange"));
            ExchangeMenu->addAction(ImportStlAction);
            QAction* StepAction = ExchangeMenu->addAction(QStringLiteral("Import STEP / IGES"));
            QAction* GltfAction = ExchangeMenu->addAction(QStringLiteral("Import glTF / GLB"));
            QAction* JtAction = ExchangeMenu->addAction(QStringLiteral("Import JT / 3DXML"));
            StepAction->setEnabled(false);
            GltfAction->setEnabled(false);
            JtAction->setEnabled(false);

            QMenu* CamMenu = menuBar()->addMenu(QStringLiteral("CAM"));
            CamMenu->addAction(BuildVoxelAction);
            CamMenu->addAction(PackageAction);

            QMenu* ViewMenu = menuBar()->addMenu(QStringLiteral("View"));
            ViewMenu->addAction(QStringLiteral("Fit All"));
            ViewMenu->addAction(QStringLiteral("Shaded with Edges"));
            ViewMenu->addAction(QStringLiteral("Section Preview"));
        }

        QWidget* CreateRibbonButton(const QString& Text, QAction* Action, const QString& Hint = QString())
        {
            QToolButton* Button = new QToolButton(this);
            Button->setText(Text);
            Button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            Button->setMinimumSize(112, 54);
            Button->setObjectName(QStringLiteral("RibbonButton"));
            if (Action)
            {
                Button->setDefaultAction(Action);
                Button->setText(Text);
            }
            if (!Hint.isEmpty())
            {
                Button->setToolTip(Hint);
            }
            return Button;
        }

        QGroupBox* CreateRibbonGroup(const QString& Title, const QList<QWidget*>& Children)
        {
            QGroupBox* Group = new QGroupBox(Title, this);
            Group->setObjectName(QStringLiteral("RibbonGroup"));
            QHBoxLayout* Layout = new QHBoxLayout(Group);
            Layout->setContentsMargins(8, 8, 8, 8);
            Layout->setSpacing(6);
            for (QWidget* Child : Children)
            {
                Layout->addWidget(Child);
            }
            Layout->addStretch(1);
            return Group;
        }

        QWidget* BuildHomeRibbon()
        {
            QWidget* Page = new QWidget(this);
            QHBoxLayout* Layout = new QHBoxLayout(Page);
            Layout->setContentsMargins(8, 6, 8, 6);
            Layout->setSpacing(8);
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Import"), {
                CreateRibbonButton(QStringLiteral("STL\nFast Import"), ImportStlAction, QStringLiteral("Binary STL high-volume import path")),
                CreateRibbonButton(QStringLiteral("STEP\nPlanned"), nullptr),
                CreateRibbonButton(QStringLiteral("glTF\nPlanned"), nullptr)
            }));
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Manufacturing"), {
                CreateRibbonButton(QStringLiteral("Build\nHex Mesh"), BuildVoxelAction, QStringLiteral("Generate hex voxel mesh for later simulation/CAM workflows")),
                CreateRibbonButton(QStringLiteral("Stock\nSetup"), nullptr),
                CreateRibbonButton(QStringLiteral("Toolpath\nSetup"), nullptr)
            }));
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Validation"), {
                CreateRibbonButton(QStringLiteral("Benchmark\nModel"), nullptr),
                CreateRibbonButton(QStringLiteral("Memory\nBudget"), nullptr),
                CreateRibbonButton(QStringLiteral("Package\nJob"), PackageAction)
            }));
            Layout->addStretch(1);
            return Page;
        }

        QWidget* BuildExchangeRibbon()
        {
            QWidget* Page = new QWidget(this);
            QHBoxLayout* Layout = new QHBoxLayout(Page);
            Layout->setContentsMargins(8, 6, 8, 6);
            Layout->setSpacing(8);
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Open Mesh"), {
                CreateRibbonButton(QStringLiteral("STL"), ImportStlAction),
                CreateRibbonButton(QStringLiteral("OBJ"), nullptr),
                CreateRibbonButton(QStringLiteral("PLY"), nullptr)
            }));
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Open CAD"), {
                CreateRibbonButton(QStringLiteral("STEP"), nullptr),
                CreateRibbonButton(QStringLiteral("JT"), nullptr),
                CreateRibbonButton(QStringLiteral("3DXML"), nullptr)
            }));
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Open BIM/CAE"), {
                CreateRibbonButton(QStringLiteral("IFC"), nullptr),
                CreateRibbonButton(QStringLiteral("VTK"), nullptr),
                CreateRibbonButton(QStringLiteral("Cache"), nullptr)
            }));
            Layout->addStretch(1);
            return Page;
        }

        QWidget* BuildCamRibbon()
        {
            QWidget* Page = new QWidget(this);
            QHBoxLayout* Layout = new QHBoxLayout(Page);
            Layout->setContentsMargins(8, 6, 8, 6);
            Layout->setSpacing(8);
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Voxel/CAM"), {
                CreateRibbonButton(QStringLiteral("Hex\nVoxel"), BuildVoxelAction),
                CreateRibbonButton(QStringLiteral("3-Axis\nRoughing"), nullptr),
                CreateRibbonButton(QStringLiteral("Waterline\nFinish"), nullptr)
            }));
            Layout->addWidget(CreateRibbonGroup(QStringLiteral("Simulation"), {
                CreateRibbonButton(QStringLiteral("Stock\nPreview"), nullptr),
                CreateRibbonButton(QStringLiteral("Collision\nCheck"), nullptr),
                CreateRibbonButton(QStringLiteral("NC\nPost"), nullptr)
            }));
            Layout->addStretch(1);
            return Page;
        }

        void CreateMainLayout()
        {
            QWidget* Central = new QWidget(this);
            QVBoxLayout* Root = new QVBoxLayout(Central);
            Root->setContentsMargins(0, 0, 0, 0);
            Root->setSpacing(0);

            QTabWidget* Ribbon = new QTabWidget(Central);
            Ribbon->setObjectName(QStringLiteral("Ribbon"));
            Ribbon->setDocumentMode(true);
            Ribbon->addTab(BuildHomeRibbon(), QStringLiteral("Home"));
            Ribbon->addTab(BuildExchangeRibbon(), QStringLiteral("Exchange"));
            Ribbon->addTab(BuildCamRibbon(), QStringLiteral("CAM / Simulation"));

            QWidget* WorkArea = new QWidget(Central);
            QVBoxLayout* WorkLayout = new QVBoxLayout(WorkArea);
            WorkLayout->setContentsMargins(8, 8, 8, 8);
            WorkLayout->setSpacing(8);

            QHBoxLayout* Cards = new QHBoxLayout();
            TriangleCard = new FMetricCard(QStringLiteral("Triangles"), QStringLiteral("0"), WorkArea);
            MemoryCard = new FMetricCard(QStringLiteral("CPU Mesh Budget"), QStringLiteral("0 B"), WorkArea);
            VoxelCard = new FMetricCard(QStringLiteral("Voxel Grid"), QStringLiteral("Not built"), WorkArea);
            HexCard = new FMetricCard(QStringLiteral("Hex Cells"), QStringLiteral("0"), WorkArea);
            Cards->addWidget(TriangleCard);
            Cards->addWidget(MemoryCard);
            Cards->addWidget(VoxelCard);
            Cards->addWidget(HexCard);
            Cards->addStretch(1);

            Viewport = new FStudioViewport(WorkArea);
            WorkLayout->addLayout(Cards);
            WorkLayout->addWidget(Viewport, 1);

            Root->addWidget(Ribbon);
            Root->addWidget(WorkArea, 1);
            setCentralWidget(Central);
        }

        QDockWidget* CreateDock(const QString& Title, QWidget* Content, Qt::DockWidgetArea Area)
        {
            QDockWidget* Dock = new QDockWidget(Title, this);
            Dock->setObjectName(Title.simplified().remove(QStringLiteral(" ")));
            Dock->setWidget(Content);
            Dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
            addDockWidget(Area, Dock);
            return Dock;
        }

        void CreateDocks()
        {
            ProjectTree = new QTreeWidget(this);
            ProjectTree->setHeaderLabels({QStringLiteral("Project / Source")});
            ProjectTree->setObjectName(QStringLiteral("DarkTree"));
            CreateDock(QStringLiteral("Project Navigator"), ProjectTree, Qt::LeftDockWidgetArea);

            OperationTree = new QTreeWidget(this);
            OperationTree->setHeaderLabels({QStringLiteral("Operation"), QStringLiteral("State")});
            OperationTree->setObjectName(QStringLiteral("DarkTree"));
            CreateDock(QStringLiteral("CAM Operation Browser"), OperationTree, Qt::LeftDockWidgetArea);

            QWidget* PropertyPanel = new QWidget(this);
            QVBoxLayout* PropertyLayout = new QVBoxLayout(PropertyPanel);
            PropertyLayout->setContentsMargins(6, 6, 6, 6);
            PropertyLayout->setSpacing(8);

            BackendCombo = new QComboBox(PropertyPanel);
            BackendCombo->addItems({QStringLiteral("Skylark Large Mesh"), QStringLiteral("VTK/ParaView Bridge"), QStringLiteral("OCCViewer Precise CAD")});
            BackendCombo->setCurrentIndex(0);
            QObject::connect(BackendCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int) {
                BackendText->setText(QStringLiteral("Backend: %1").arg(BackendCombo->currentText()));
                AppendLog(QStringLiteral("[VIEW] Backend route selected: %1").arg(BackendCombo->currentText()));
            });

            ImportProgress = new QProgressBar(PropertyPanel);
            ImportProgress->setRange(0, 100);
            ImportProgress->setValue(0);
            ImportProgress->setTextVisible(true);

            PropertyTable = CreateTable({QStringLiteral("Property"), QStringLiteral("Value")}, PropertyPanel);
            StatisticsTable = CreateTable({QStringLiteral("Metric"), QStringLiteral("Value")}, PropertyPanel);

            PropertyLayout->addWidget(new QLabel(QStringLiteral("Display Backend"), PropertyPanel));
            PropertyLayout->addWidget(BackendCombo);
            PropertyLayout->addWidget(new QLabel(QStringLiteral("Import / Compute Progress"), PropertyPanel));
            PropertyLayout->addWidget(ImportProgress);
            PropertyLayout->addWidget(new QLabel(QStringLiteral("Object Properties"), PropertyPanel));
            PropertyLayout->addWidget(PropertyTable, 1);
            PropertyLayout->addWidget(new QLabel(QStringLiteral("Performance Statistics"), PropertyPanel));
            PropertyLayout->addWidget(StatisticsTable, 1);
            CreateDock(QStringLiteral("Properties / Statistics"), PropertyPanel, Qt::RightDockWidgetArea);

            QWidget* ManufacturingPanel = new QWidget(this);
            QVBoxLayout* ManufacturingLayout = new QVBoxLayout(ManufacturingPanel);
            ManufacturingLayout->setContentsMargins(6, 6, 6, 6);
            ManufacturingLayout->setSpacing(8);
            ToolTable = CreateTable({QStringLiteral("Tool"), QStringLiteral("Diameter"), QStringLiteral("Use")}, ManufacturingPanel);
            JobTable = CreateTable({QStringLiteral("Stage"), QStringLiteral("Status"), QStringLiteral("Output")}, ManufacturingPanel);
            ManufacturingLayout->addWidget(new QLabel(QStringLiteral("Tool Library"), ManufacturingPanel));
            ManufacturingLayout->addWidget(ToolTable, 1);
            ManufacturingLayout->addWidget(new QLabel(QStringLiteral("Manufacturing Job Queue"), ManufacturingPanel));
            ManufacturingLayout->addWidget(JobTable, 1);
            CreateDock(QStringLiteral("Manufacturing Setup"), ManufacturingPanel, Qt::RightDockWidgetArea);

            Console = new QTextEdit(this);
            Console->setReadOnly(true);
            Console->setObjectName(QStringLiteral("Console"));
            CreateDock(QStringLiteral("Diagnostics Console"), Console, Qt::BottomDockWidgetArea);

            SeedToolTable();
            SeedJobTable();
        }

        QTableWidget* CreateTable(const QStringList& Headers, QWidget* Parent)
        {
            QTableWidget* Table = new QTableWidget(Parent);
            Table->setColumnCount(Headers.size());
            Table->setHorizontalHeaderLabels(Headers);
            Table->horizontalHeader()->setStretchLastSection(true);
            Table->verticalHeader()->setVisible(false);
            Table->setSelectionBehavior(QAbstractItemView::SelectRows);
            Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            Table->setAlternatingRowColors(true);
            Table->setObjectName(QStringLiteral("DarkTable"));
            return Table;
        }

        void CreateStatusBar()
        {
            StatusText = new QLabel(QStringLiteral("Ready"), this);
            BackendText = new QLabel(QStringLiteral("Backend: Skylark Large Mesh"), this);
            MemoryBudgetText = new QLabel(QStringLiteral("Target: 600M STL / 20M triangles / CAM payload path"), this);
            statusBar()->addWidget(StatusText, 1);
            statusBar()->addPermanentWidget(BackendText);
            statusBar()->addPermanentWidget(MemoryBudgetText);
        }

        void ApplyCommercialStyle()
        {
            qApp->setStyle(QStringLiteral("Fusion"));
            const QString Style = QStringLiteral(R"(
                QMainWindow, QWidget { background: #111827; color: #dbe7f3; font-family: 'Segoe UI', 'Microsoft YaHei UI', sans-serif; font-size: 9pt; }
                QMenuBar { background: #0b1220; color: #e5edf7; border-bottom: 1px solid #263344; }
                QMenuBar::item:selected { background: #1f2e42; }
                QMenu { background: #111827; border: 1px solid #2a3a50; }
                QMenu::item:selected { background: #23415d; }
                QDockWidget { titlebar-close-icon: none; titlebar-normal-icon: none; }
                QDockWidget::title { background: #172235; padding: 6px; border: 1px solid #27364b; font-weight: 600; }
                QTabWidget::pane { border-top: 1px solid #27364b; background: #111827; }
                QTabBar::tab { background: #172235; color: #dbe7f3; padding: 8px 16px; border: 1px solid #27364b; border-bottom: none; }
                QTabBar::tab:selected { background: #22324a; color: #ffffff; }
                QGroupBox#RibbonGroup { background: #151f2e; border: 1px solid #2b3c55; border-radius: 6px; margin-top: 10px; padding-top: 8px; font-weight: 600; color: #9fc7ff; }
                QGroupBox#RibbonGroup::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }
                QToolButton#RibbonButton { background: #22324a; border: 1px solid #31465f; border-radius: 6px; padding: 5px; color: #eef6ff; }
                QToolButton#RibbonButton:hover { background: #2a4768; border-color: #4f88c7; }
                QToolButton#RibbonButton:disabled { color: #6f7c8e; background: #1a2433; }
                QFrame#MetricCard { background: #162235; border: 1px solid #2a3b53; border-radius: 10px; }
                QLabel#MetricTitle { color: #93a4b8; font-size: 8pt; }
                QLabel#MetricValue { color: #ffffff; font-size: 15pt; font-weight: 700; }
                QTreeWidget#DarkTree, QTableWidget#DarkTable, QTextEdit#Console { background: #0c1320; alternate-background-color: #111c2b; color: #dce7f3; border: 1px solid #29384d; selection-background-color: #315f8c; gridline-color: #2b3a50; }
                QHeaderView::section { background: #1b2b40; color: #e6eef8; padding: 5px; border: 1px solid #2d3d55; font-weight: 600; }
                QProgressBar { background: #0c1320; border: 1px solid #2d3d55; border-radius: 4px; text-align: center; color: #e6eef8; }
                QProgressBar::chunk { background: #2f80ed; border-radius: 3px; }
                QComboBox { background: #0c1320; border: 1px solid #2d3d55; border-radius: 4px; padding: 4px; }
                QStatusBar { background: #0b1220; color: #bcd0e5; border-top: 1px solid #27364b; }
                QLabel { color: #dbe7f3; }
            )");
            qApp->setStyleSheet(Style);
        }

        void ResetProjectTree()
        {
            ProjectTree->clear();
            QTreeWidgetItem* Root = new QTreeWidgetItem(ProjectTree, {QStringLiteral("Skylark Manufacturing Project")});
            new QTreeWidgetItem(Root, {QStringLiteral("External Sources")});
            new QTreeWidgetItem(Root, {QStringLiteral("Model Payload Repository")});
            new QTreeWidgetItem(Root, {QStringLiteral("CAM Setups")});
            new QTreeWidgetItem(Root, {QStringLiteral("Simulation Meshes")});
            Root->setExpanded(true);
        }

        void ResetOperationTree()
        {
            OperationTree->clear();
            QTreeWidgetItem* Setup = new QTreeWidgetItem(OperationTree, {QStringLiteral("Setup 1 - Imported Mesh CAM"), QStringLiteral("Planned")});
            new QTreeWidgetItem(Setup, {QStringLiteral("1. Source Model"), QStringLiteral("Waiting")});
            new QTreeWidgetItem(Setup, {QStringLiteral("2. Hex Voxel Mesh"), QStringLiteral("Waiting")});
            new QTreeWidgetItem(Setup, {QStringLiteral("3. Stock Model"), QStringLiteral("Reserved")});
            new QTreeWidgetItem(Setup, {QStringLiteral("4. Roughing Toolpath"), QStringLiteral("Reserved")});
            new QTreeWidgetItem(Setup, {QStringLiteral("5. Simulation / Collision"), QStringLiteral("Reserved")});
            Setup->setExpanded(true);
        }

        void SeedToolTable()
        {
            SetTableRows(ToolTable, {
                {QStringLiteral("Flat End Mill"), QStringLiteral("D10"), QStringLiteral("Roughing")},
                {QStringLiteral("Ball End Mill"), QStringLiteral("R6"), QStringLiteral("Finishing")},
                {QStringLiteral("Bull Nose Mill"), QStringLiteral("D8 R1"), QStringLiteral("Rest machining")}
            });
        }

        void SeedJobTable()
        {
            SetTableRows(JobTable, {
                {QStringLiteral("Import"), QStringLiteral("Idle"), QStringLiteral("LargeMeshPayload")},
                {QStringLiteral("Voxelization"), QStringLiteral("Idle"), QStringLiteral("HexahedralMesh")},
                {QStringLiteral("CAM Toolpath"), QStringLiteral("Reserved"), QStringLiteral("NCOperation")},
                {QStringLiteral("Post Process"), QStringLiteral("Reserved"), QStringLiteral("G-Code")}
            });
        }

        void SetTableRows(QTableWidget* Table, const QList<QStringList>& Rows)
        {
            Table->setRowCount(Rows.size());
            for (int Row = 0; Row < Rows.size(); ++Row)
            {
                for (int Column = 0; Column < Rows[Row].size(); ++Column)
                {
                    Table->setItem(Row, Column, new QTableWidgetItem(Rows[Row][Column]));
                }
            }
            Table->resizeColumnsToContents();
        }

        void UpdateMetricsEmpty()
        {
            TriangleCard->SetValue(QStringLiteral("0"));
            MemoryCard->SetValue(QStringLiteral("0 B"));
            VoxelCard->SetValue(QStringLiteral("Not built"));
            HexCard->SetValue(QStringLiteral("0"));
            SetTableRows(PropertyTable, {
                {QStringLiteral("Model State"), QStringLiteral("No source model loaded")},
                {QStringLiteral("Display Route"), QStringLiteral("Skylark Large Mesh / future GPU resident buffer")},
                {QStringLiteral("CAM Geometry"), QStringLiteral("Mesh CAM Provider planned")}
            });
            SetTableRows(StatisticsTable, {
                {QStringLiteral("Triangle Count"), QStringLiteral("0")},
                {QStringLiteral("CPU Mesh Bytes"), QStringLiteral("0 B")},
                {QStringLiteral("Voxel Cells"), QStringLiteral("0")},
                {QStringLiteral("Hex Cells"), QStringLiteral("0")}
            });
        }

        void AppendLog(const QString& Message)
        {
            const QString Line = QStringLiteral("%1  %2")
                                     .arg(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss")))
                                     .arg(Message);
            Console->append(Line);
            StatusText->setText(Message);
        }

        void SetJobStatus(const int Row, const QString& Status)
        {
            if (JobTable && Row >= 0 && Row < JobTable->rowCount())
            {
                JobTable->setItem(Row, 1, new QTableWidgetItem(Status));
            }
        }

        void OpenBinaryStl()
        {
            const QString FileName = QFileDialog::getOpenFileName(this,
                                                                  QStringLiteral("Import Binary STL - Large Mesh Payload"),
                                                                  QString(),
                                                                  QStringLiteral("STL Files (*.stl);;All Files (*.*)"));
            if (FileName.isEmpty())
            {
                return;
            }

            ImportProgress->setValue(5);
            SetJobStatus(0, QStringLiteral("Reading"));
            AppendLog(QStringLiteral("[IMPORT] Reading binary STL through high-volume payload path: %1").arg(FileName));

            FSKStlFastReaderOptions Options;
            Options.bStoreFacetNormals = false;
            const FSKStlFastReaderOutput Output = FSKStlFastReader::ReadBinaryFile(FileName.toStdString(), Options);
            if (!Output.Result.IsOk())
            {
                ImportProgress->setValue(0);
                SetJobStatus(0, QStringLiteral("Failed"));
                AppendLog(QStringLiteral("[ERROR] %1").arg(QString::fromStdString(Output.Result.Message)));
                return;
            }

            CurrentMesh = std::make_unique<FSKVolumeStudioMeshAsset>(Output.Mesh);
            CurrentHexMesh.reset();
            CurrentVoxelGrid = FSKVolumeVoxelGrid();
            CurrentFileName = FileName;
            CurrentBaseName = QFileInfo(FileName).fileName();
            ImportProgress->setValue(100);
            SetJobStatus(0, QStringLiteral("Completed"));
            SetJobStatus(1, QStringLiteral("Ready"));
            BuildVoxelAction->setEnabled(CurrentMesh->GetTriangleCount() > 0);
            PackageAction->setEnabled(true);

            UpdateAfterMeshImport();
            AppendLog(QStringLiteral("[OK] Loaded STL: triangles=%1, cpuMesh=%2")
                          .arg(FormatCount(CurrentMesh->GetTriangleCount()))
                          .arg(FormatBytes(CurrentMesh->EstimateMemoryBytes())));
        }

        void UpdateAfterMeshImport()
        {
            if (!CurrentMesh)
            {
                return;
            }

            TriangleCard->SetValue(FormatCount(CurrentMesh->GetTriangleCount()));
            MemoryCard->SetValue(FormatBytes(CurrentMesh->EstimateMemoryBytes()));
            VoxelCard->SetValue(QStringLiteral("Ready"));
            HexCard->SetValue(QStringLiteral("0"));

            ResetProjectTree();
            QTreeWidgetItem* Root = ProjectTree->topLevelItem(0);
            QTreeWidgetItem* Sources = Root->child(0);
            QTreeWidgetItem* Payloads = Root->child(1);
            QTreeWidgetItem* SourceItem = new QTreeWidgetItem(Sources, {CurrentBaseName});
            new QTreeWidgetItem(SourceItem, {QStringLiteral("Format: Binary STL")});
            new QTreeWidgetItem(SourceItem, {QStringLiteral("Policy: external mesh payload")});
            new QTreeWidgetItem(Payloads, {QStringLiteral("LargeMeshPayload: triangles=%1").arg(FormatCount(CurrentMesh->GetTriangleCount()))});
            Root->setExpanded(true);
            Sources->setExpanded(true);
            Payloads->setExpanded(true);

            OperationTree->topLevelItem(0)->child(0)->setText(1, QStringLiteral("Completed"));
            OperationTree->topLevelItem(0)->child(1)->setText(1, QStringLiteral("Ready"));

            const FSKVolumeStudioAabb Bounds = CurrentMesh->GetBounds();
            Viewport->SetModelState(CurrentBaseName, CurrentMesh->GetTriangleCount(), Bounds, true);

            SetTableRows(PropertyTable, {
                {QStringLiteral("Source File"), CurrentFileName},
                {QStringLiteral("Format"), QStringLiteral("Binary STL")},
                {QStringLiteral("Payload Policy"), QStringLiteral("Large mesh payload, no OCCT BRep conversion")},
                {QStringLiteral("Facet Normals"), QStringLiteral("Skipped to reduce memory")},
                {QStringLiteral("CAM Route"), QStringLiteral("Mesh CAM / voxel simulation path")},
                {QStringLiteral("Render Route"), BackendCombo->currentText()}
            });

            SetTableRows(StatisticsTable, {
                {QStringLiteral("Triangle Count"), FormatCount(CurrentMesh->GetTriangleCount())},
                {QStringLiteral("CPU Mesh Bytes"), FormatBytes(CurrentMesh->EstimateMemoryBytes())},
                {QStringLiteral("Bounds Min"), QStringLiteral("%1, %2, %3").arg(Bounds.Min.X).arg(Bounds.Min.Y).arg(Bounds.Min.Z)},
                {QStringLiteral("Bounds Max"), QStringLiteral("%1, %2, %3").arg(Bounds.Max.X).arg(Bounds.Max.Y).arg(Bounds.Max.Z)},
                {QStringLiteral("Display Cache"), QStringLiteral("Skylark bridge preview generated on demand")},
                {QStringLiteral("Target Upgrade"), QStringLiteral("Chunk + GPU resident buffer + BVH + LOD")}
            });
        }

        void BuildVoxelMesh()
        {
            if (!CurrentMesh)
            {
                AppendLog(QStringLiteral("[ERROR] No mesh loaded."));
                return;
            }

            ImportProgress->setValue(15);
            SetJobStatus(1, QStringLiteral("Voxelizing"));
            AppendLog(QStringLiteral("[VOXEL] Building conservative surface voxel grid for simulation/CAM pre-processing."));

            FSKVoxelBuildSettings Settings;
            Settings.MaxResolutionPerAxis = 96;
            const FSKVoxelizationOutput VoxelOutput = FSKSurfaceVoxelizer::BuildConservativeVoxelGrid(*CurrentMesh, Settings);
            if (!VoxelOutput.Result.IsOk())
            {
                ImportProgress->setValue(0);
                SetJobStatus(1, QStringLiteral("Failed"));
                AppendLog(QStringLiteral("[ERROR] %1").arg(QString::fromStdString(VoxelOutput.Result.Message)));
                return;
            }

            ImportProgress->setValue(72);
            const FSKHexahedralMeshBuildOutput HexOutput = FSKHexahedralMeshBuilder::BuildFromOccupiedVoxels(VoxelOutput.Grid);
            if (!HexOutput.Result.IsOk())
            {
                ImportProgress->setValue(0);
                SetJobStatus(1, QStringLiteral("Failed"));
                AppendLog(QStringLiteral("[ERROR] %1").arg(QString::fromStdString(HexOutput.Result.Message)));
                return;
            }

            CurrentVoxelGrid = VoxelOutput.Grid;
            CurrentHexMesh = std::make_unique<FSKHexahedralMesh>(HexOutput.Mesh);
            ImportProgress->setValue(100);
            SetJobStatus(1, QStringLiteral("Completed"));
            SetJobStatus(2, QStringLiteral("Reserved"));

            VoxelCard->SetValue(QStringLiteral("%1 x %2 x %3")
                                    .arg(CurrentVoxelGrid.DimX)
                                    .arg(CurrentVoxelGrid.DimY)
                                    .arg(CurrentVoxelGrid.DimZ));
            HexCard->SetValue(FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetCellCount())));
            Viewport->SetVoxelState(CurrentVoxelGrid, *CurrentHexMesh, true);

            QTreeWidgetItem* Root = ProjectTree->topLevelItem(0);
            QTreeWidgetItem* Simulation = Root->child(3);
            Simulation->takeChildren();
            new QTreeWidgetItem(Simulation, {QStringLiteral("HexMesh: cells=%1, nodes=%2")
                                                 .arg(FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetCellCount())))
                                                 .arg(FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetNodeCount())))});
            Simulation->setExpanded(true);

            SetTableRows(StatisticsTable, {
                {QStringLiteral("Triangle Count"), FormatCount(CurrentMesh->GetTriangleCount())},
                {QStringLiteral("CPU Mesh Bytes"), FormatBytes(CurrentMesh->EstimateMemoryBytes())},
                {QStringLiteral("Voxel Dims"), QStringLiteral("%1 x %2 x %3").arg(CurrentVoxelGrid.DimX).arg(CurrentVoxelGrid.DimY).arg(CurrentVoxelGrid.DimZ)},
                {QStringLiteral("Boundary Cells"), FormatCount(static_cast<std::uint64_t>(VoxelOutput.Stats.BoundaryCellCount))},
                {QStringLiteral("Solid Cells"), FormatCount(static_cast<std::uint64_t>(VoxelOutput.Stats.SolidCellCount))},
                {QStringLiteral("Hex Cells"), FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetCellCount()))},
                {QStringLiteral("Hex Nodes"), FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetNodeCount()))}
            });

            AppendLog(QStringLiteral("[OK] Voxel grid %1 x %2 x %3, boundary=%4, solid=%5, hexCells=%6, hexNodes=%7")
                          .arg(CurrentVoxelGrid.DimX)
                          .arg(CurrentVoxelGrid.DimY)
                          .arg(CurrentVoxelGrid.DimZ)
                          .arg(FormatCount(static_cast<std::uint64_t>(VoxelOutput.Stats.BoundaryCellCount)))
                          .arg(FormatCount(static_cast<std::uint64_t>(VoxelOutput.Stats.SolidCellCount)))
                          .arg(FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetCellCount())))
                          .arg(FormatCount(static_cast<std::uint64_t>(CurrentHexMesh->GetNodeCount()))));
        }
    };
}

int main(int argc, char** argv)
{
    QApplication App(argc, argv);
    FVolumeStudioWindow Window;
    Window.show();
    return App.exec();
}
