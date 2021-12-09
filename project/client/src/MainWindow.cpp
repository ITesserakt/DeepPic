#include "MainWindow.h"
#include "QGraphicsView"
#include "Palette.h"
#include "Changer.h"

#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QIcon>
#include <QPixmap>

// Temporary
#include <fstream>

#include <stdexcept>

////
#include <QImage>
#include <QPixmap>
////

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    scene = new PaintScene(this);
    connect(scene, &PaintScene::writeCurveSignal, this, &MainWindow::writeSlot);
    connect(this, &MainWindow::addCurve, scene, &PaintScene::readCurveSlot);

    canvas = new Canvas;
    canvas->setScene(scene);

    setCentralWidget(canvas);

    // MenuBar
    auto *file_open = new QAction("&Open", this);
    connect(file_open, &QAction::triggered, canvas, &Canvas::openImageSlot);
    auto *file_close = new QAction("&Close", this);
    connect(file_close, &QAction::triggered, canvas, &Canvas::closeImageSlot);
    auto *file_save = new QAction("&Save", this);
    connect(file_save, &QAction::triggered, canvas, &Canvas::saveImageSlot);
    auto *file_save_as = new QAction("&Save as", this);
    connect(file_save_as, &QAction::triggered, canvas, &Canvas::saveAsImageSlot);
    QMenu *file;
    file = menuBar()->addMenu("File");
    file->addAction(file_open);
    file->addAction(file_close);
    file->addAction(file_save);
    file->addAction(file_save_as);

    auto *edit_undo = new QAction("&Undo", this);
    auto *edit_redo = new QAction("&Redo", this);
    QMenu *edit;
    edit = menuBar()->addMenu("Edit");
    edit->addAction(edit_undo);
    edit->addAction(edit_redo);

    auto *help_help = new QAction("&Help", this);
    QMenu *help;
    help = menuBar()->addMenu("Help");
    help->addAction(help_help);

    // ToolBars
    parametersPanel = new ParametersPanel(this);
    addToolBar(parametersPanel);
    parameters_panel = new QToolBar;
    addToolBar(parameters_panel);
    parameters_panel->setFixedHeight(40);

    connector = new Connector(this);
    addToolBar(Qt::RightToolBarArea, connector);
    connector->setFixedWidth(400);

    connect(connector, &Connector::writeSignal, this, &MainWindow::writeSlot);
    connect(this, &MainWindow::failedShareSignal, connector, &Connector::failedShareSlot);
    connect(this, &MainWindow::failedConnectSignal, connector, &Connector::failedConnectSlot);
    //connect(this, &MainWindow::successfulConnectSignal, connector, &Connector::successfulConnectSlot); // TODO
    connect(this, &MainWindow::successfulShareSignal, connector, &Connector::successfulShareSlot);



    toolsPanel = new ToolsPanel;
    addToolBar(Qt::LeftToolBarArea, toolsPanel);

    connect(toolsPanel, &ToolsPanel::BrushTriggered, this, &MainWindow::slotBrush);
    //connect(this, &MainWindow::addCurve, scene, &PaintScene::PaintCurveSlot);
    //connect(scene, &PaintScene::PushCurve, this, &MainWindow::TemporaryWriterSlot);


    //  Status bar
    statusBar()->showMessage("Status bar");

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(500);
    // TODO: client
}

MainWindow::~MainWindow() {
}

void MainWindow::slotTimer() {
    scene->setSceneRect(0, 0, canvas->width() - 20, canvas->height() - 20);
}

void MainWindow::slotBrush(qreal brushSize, const QColor& brushColor) {

    if (scene->BrushStatus()) {
        parametersPanel->clear();
    } else {
        scene->SetBrush(brushSize, brushColor);
        parametersPanel->setBrush(brushSize, brushColor);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushSizeChanged), scene, &PaintScene::SetBrushSize);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushSizeChanged), toolsPanel, &ToolsPanel::SetBrushSizeSlot);

        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushRedChanged), scene, &PaintScene::SetRedSlot);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushRedChanged), toolsPanel, &ToolsPanel::SetBrushRedSlot);

        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushGreenChanged), scene, &PaintScene::SetGreenSlot);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushGreenChanged), toolsPanel, &ToolsPanel::SetBrushGreenSlot);

        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushBlueChanged), scene, &PaintScene::SetBlueSlot);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushBlueChanged), toolsPanel, &ToolsPanel::SetBrushBlueSlot);

        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushOpacityChanged), scene, &PaintScene::SetTransparencySlot);
        connect(parametersPanel, QOverload<int>::of(&ParametersPanel::BrushOpacityChanged), toolsPanel, &ToolsPanel::SetBrushOpacitySlot);
    }
    scene->ChangeBrushStatus();
}

//void MainWindow::executeBrush(const Curve& curve) {
//    if (curve.brush_size < 0) {
//        throw std::invalid_argument("Invalid size");
//    }
//    if (curve.color_red < 0 || curve.color_red > 255) {
//        throw std::invalid_argument("Invalid red color");
//    }
//    if (curve.color_green < 0 || curve.color_green > 255) {
//        throw std::invalid_argument("Invalid green color");
//    }
//    if (curve.color_blue < 0 || curve.color_blue > 255) {
//        throw std::invalid_argument("Invalid blue color");
//    }
//    if (curve.coords.size() < 2) {
//        throw std::invalid_argument("Invalid curve size");
//    }
//    emit(addCurve(curve));
//}
void MainWindow::execute(std::string&& message) {
    // TODO: add parser

    // TODO: if sharing failed
    if (false) {
        emit(failedShareSignal());
    }

    // TODO: if sharing succeeded
    if (false) {
        std::string message = ""; // TODO: add message format
        emit(successfulShareSignal(message));
    }

    // TODO: if joining failed
    if (false) {
        emit(failedConnectSignal());
    }

    // TODO: if joining succeeded
    if (false) {
        std::string message = ""; // TODO: add message format
        emit(successfulConnectSignal(message));
    }

    // TODO: if need to draw a line
    if (false) {
        std::string message = "";// TODO: add parser message->curve
        emit(addCurve(message));
    }

}
void MainWindow::writeSlot(std::string &message) {
    // TODO: client write
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
