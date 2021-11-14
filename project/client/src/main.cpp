#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(800, 800);
    window.setWindowTitle("DeepPic");
    window.showMaximized();

    return app.exec();
}