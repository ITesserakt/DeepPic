#include "MainWindow.h"
#include "Changer.h"
#include "Palette.h"
#include "QGraphicsView"

#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QStatusBar>
#include <QToolBar>

// Temporary
#include <fstream>

#include <stdexcept>

////
#include <QImage>
#include <QPixmap>
////

#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent) {

    scene = new PaintScene(this);
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

    auto *right_panel = new QToolBar;
    addToolBar(Qt::RightToolBarArea, right_panel);
    right_panel->setFixedWidth(400);

    toolsPanel = new ToolsPanel;
    addToolBar(Qt::LeftToolBarArea, toolsPanel);

    connect(toolsPanel, &ToolsPanel::BrushTriggered, this, &MainWindow::slotBrush);
    connect(this, &MainWindow::TemporarySignal, scene, &PaintScene::PaintCurveSlot);
    //connect(scene, &PaintScene::PushCurve, this, &MainWindow::TemporaryWriterSlot);


    //  Status bar
    statusBar()->showMessage("Status bar");

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(500);

    serverConnection_ = std::make_unique<ServerConnection>(std::string("127.0.0.1"), 8080, ServerConnectionCallbacks{
            [this](std::string &&message) { this->execute(std::move(message)); },
            {}
    });

    serverConnection_->start();

    json test_target = {{"target", "sharing_document"}};
    serverConnection_->write(test_target.dump());
}

MainWindow::~MainWindow() {
}

void MainWindow::slotTimer() {
    //scene->setSceneRect(0, 0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);
}

void MainWindow::slotBrush(qreal brushSize, const QColor &brushColor) {

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

void MainWindow::executeBrush(const Curve &curve) {
    if (curve.brush_size < 0) {
        throw std::invalid_argument("Invalid size");
    }
    if (curve.color_red < 0 || curve.color_red > 255) {
        throw std::invalid_argument("Invalid red color");
    }
    if (curve.color_green < 0 || curve.color_green > 255) {
        throw std::invalid_argument("Invalid green color");
    }
    if (curve.color_blue < 0 || curve.color_blue > 255) {
        throw std::invalid_argument("Invalid blue color");
    }
    if (curve.coords.size() < 2) {
        throw std::invalid_argument("Invalid curve size");
    }
    emit(TemporarySignal(curve));
}

void MainWindow::execute(std::string &&message) {
    std::cout << "MainWindow::execute(), message = " << message << std::endl;
    try {
        auto command = json::parse(message);
        if (command["target"] == "sharing_document" && command["status"] == "OK") {
            forDocumentConnection_ = std::make_unique<ServerConnection>(std::string("127.0.0.1"), 8080, ServerConnectionCallbacks{
                    [this](std::string &&message) { this->execute(std::move(message)); },
                    {}
            });
            forDocumentConnection_->setServerPort(command["port"]);
            forDocumentConnection_->setServerUrl(command["address"]);
            forDocumentConnection_->start();

            json command_json = {{"target",     "auth"},
                                 {"auth_token", command["auth_token"]}};
            forDocumentConnection_->write(command_json.dump() + std::string("\r\n"));
        }
    } catch (...) {
        std::cerr << "some exception" << std::endl;
    }
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
