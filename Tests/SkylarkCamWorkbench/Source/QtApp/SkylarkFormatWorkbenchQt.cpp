#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QHeaderView>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "Import/ImporterRegistry.h"

using namespace Skylark::FormatWorkbench;

class FwMainWindow final : public QMainWindow
{
public:
    FwMainWindow()
    {
        setWindowTitle("Skylark Format Workbench");
        resize(1280, 760);

        auto* OpenAction = new QAction("Open 3D File", this);
        connect(OpenAction, &QAction::triggered, this, [this]() { OpenFile(); });
        QToolBar* Bar = addToolBar("File");
        Bar->addAction(OpenAction);

        auto* Central = new QWidget(this);
        auto* Layout = new QVBoxLayout(Central);
        Table = new QTableWidget(0, 2, Central);
        Table->setHorizontalHeaderLabels({ "Metric", "Value" });
        Table->horizontalHeader()->setStretchLastSection(true);
        Log = new QPlainTextEdit(Central);
        Log->setReadOnly(true);
        auto* Splitter = new QSplitter(Qt::Vertical, Central);
        Splitter->addWidget(Table);
        Splitter->addWidget(Log);
        Layout->addWidget(Splitter);
        setCentralWidget(Central);
    }

private:
    void OpenFile()
    {
        const QString FileName = QFileDialog::getOpenFileName(this, "Open model", QString(), "3D files (*.stl *.step *.stp *.obj *.ply *.gltf *.glb *.jt *.3dxml);;All files (*.*)");
        if (FileName.isEmpty())
            return;

        FwImportOptions Options;
        FwImporterRegistry Registry = CreateDefaultImporterRegistry();
        FwImportResult Result = Registry.Import(FileName.toStdString(), Options);
        DisplayReport(Result.Report);
    }

    void DisplayReport(const FwImportReport& Report)
    {
        Table->setRowCount(0);
        auto AddRow = [this](const QString& Name, const QString& Value)
        {
            const int Row = Table->rowCount();
            Table->insertRow(Row);
            Table->setItem(Row, 0, new QTableWidgetItem(Name));
            Table->setItem(Row, 1, new QTableWidgetItem(Value));
        };
        AddRow("Status", Report.Succeeded ? "PASS" : "FAIL");
        AddRow("Format", ToString(Report.Format));
        AddRow("Triangles", QString::number(static_cast<qulonglong>(Report.Triangles)));
        AddRow("Vertices", QString::number(static_cast<qulonglong>(Report.Vertices)));
        AddRow("Mesh blocks", QString::number(static_cast<qulonglong>(Report.MeshBlocks)));
        AddRow("Compact CPU bytes", QString::number(static_cast<qulonglong>(Report.CompactCpuBytes)));
        AddRow("Import milliseconds", QString::number(Report.ImportMilliseconds, 'f', 3));

        Log->clear();
        for (const FwImportMessage& Message : Report.Messages)
            Log->appendPlainText(QString::fromStdString(Message.Text));
    }

    QTableWidget* Table = nullptr;
    QPlainTextEdit* Log = nullptr;
};

int main(int Argc, char** Argv)
{
    QApplication App(Argc, Argv);
    FwMainWindow Window;
    Window.show();
    return App.exec();
}
