#include "App/MainWindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    skylark::cam::MainWindow window;
    window.show();
    return app.exec();
}
