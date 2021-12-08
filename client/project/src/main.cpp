//
// Created by tesserakt on 30.10.2021.
//

#include <QApplication>

#include "CommandBus.h"
#include "MainWindow.h"
#include "UICommandExecutor.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    CommandBus<true> globalCommandBus;
    PaintScene scene{globalCommandBus};
    MainWindow window{scene};
    globalCommandBus.addExecutor<UICommandExecutor>(scene);

    window.setMinimumSize(500, 500);
    window.setWindowTitle("DeepPic");
    window.showMaximized();

    return a.exec();
}