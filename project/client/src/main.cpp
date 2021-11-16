#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.setMinimumSize(500, 500);
    window.setWindowTitle("DeepPic");
    window.showMaximized();
    return a.exec();
}
