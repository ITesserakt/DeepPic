#include "MainWindow.h"
#include "Changer.h"
#include "Palette.h"
#include "QGraphicsView"
#include "Connector.h"

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
    connect(scene, &PaintScene::writeCurveSignal, this, &MainWindow::writeSlot);
    connect(this, &MainWindow::addCurve, scene, &PaintScene::readCurveSlot);
    connect(this, &MainWindow::setImageSignal, canvas, &Canvas::setImageSlot);

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
    connect(this, &MainWindow::successfulConnectSignal, connector, &Connector::successfulConnectSlot);
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
    serverConnection_ = std::make_unique<ServerConnection>(std::string("127.0.0.1"), 8080, ServerConnectionCallbacks{
            [this](std::string &&message) { this->execute(std::move(message)); },
            {}
    });

    serverConnection_->start();
}

MainWindow::~MainWindow() {
}

void MainWindow::slotTimer() {
    scene->setSceneRect(0, 0, canvas->width() - 16, canvas->height() - 16);
    timer->stop();
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
void MainWindow::execute(std::string &&message) {
    std::cout << "MainWindow::execute(), message = " << message << std::endl;

    // TODO: 4) add parser
    auto parse_message = json::parse(message);

    // TODO: if sharing failed
    if (parse_message.contains("status") && parse_message["target"] == "sharing_document" && parse_message["status"] == "FAIL") {
        emit(failedShareSignal());
    } else

        // TODO: if sharing succeeded
    if (parse_message.contains("status") && parse_message["target"] == "sharing_document" && parse_message["status"] == "OK") {
        std::cout << "// TODO: if sharing succeeded" << std::endl;

        if (forDocumentConnection_) {
            forDocumentConnection_->stop();
        }

        forDocumentConnection_ = std::make_unique<ServerConnection>(parse_message["address"], parse_message["port"],
                                                                    ServerConnectionCallbacks{
                                                                            [this](std::string &&message) {
                                                                                this->execute(std::move(message));
                                                                            },
                                                                            {}
                                                                    });
        std::cout << "// TODO: if sharing succeeded" << std::endl;
        forDocumentConnection_->start();
        std::cout << "// TODO: if sharing succeeded" << std::endl;
        QString auth_token = QString::fromStdString(
                to_string(parse_message["address"]) + std::string(":") + to_string(parse_message["port"]) +
                to_string(parse_message["auth_token"]));
        json to_connect = {{"target",     "auth"},
                           {"auth_token", parse_message["auth_token"]}};
        std::cout << "// TODO: if sharing succeeded" << std::endl;
        forDocumentConnection_->write(to_connect.dump());
        emit(successfulShareSignal(auth_token));
    } else

        // TODO: if joining failed
    if (parse_message.contains("status") && parse_message["target"] == "auth" && parse_message["status"] == "FAIL") {
        std::cout << "TODO: if joining failed" << std::endl;
        emit(failedConnectSignal());
    } else

        // TODO: if joining succeeded
    if (parse_message.contains("status") && parse_message["target"] == "auth" && parse_message["status"] == "OK") {
        QString auth_token = QString::fromStdString(
                to_string(parse_message["address"]) + std::string(":") + to_string(parse_message["port"]) + std::string("|") +
                to_string(parse_message["auth_token"]));
        std::cout << "// TODO: if joining succeeded" << std::endl;
        emit(successfulConnectSignal(auth_token));
    } else

        // TODO: if need to draw a line
    if (parse_message["target"] == "sharing_command") {
        std::cout << "TODO: if need to draw a line" << std::endl;
        QString command = QString::fromStdString(parse_message["command"]);
//        std::string command = "20 0 128 0 255 402 243 400 248";
        emit(addCurve(command));
        //QString msg = "30";
//        emit(addTestPoint(msg));
    }

    // TODO: if need get Image
    if (false) {
        std::vector<unsigned char> imageVector = canvas->GetImageVector().toStdVector();
    }

    // TODO: if need set Image
    if (false) {
        std::vector<unsigned char> imageVector; // <- for writing
        emit(setImageSignal(QVector<unsigned char>::fromStdVector(imageVector)));
    }
}


void MainWindow::writeSlot(std::string &message) {
    //std::string target = json::parse(message)["target"];
    auto json_message = json::parse(message);
    std::cerr << "MainWindow::writeSlot, message = " << message << std::endl;
    if (json_message["target"] == "sharing_document" && serverConnection_) {
        serverConnection_->write(std::move(message));
    } else if (forDocumentConnection_) {
        forDocumentConnection_->write(std::move(message));
    } else {
        std::string address_string = to_string(json_message["address"]);
        std::string address = address_string.substr(1, address_string.find(':') - 1);
        std::string port = address_string.substr(address_string.find(':') + 1, address_string.size() - address_string.find(':') - 2);
        std::cout << address << " " << port << std::endl;
        forDocumentConnection_ = std::make_unique<ServerConnection>(std::move(address), std::stoi(port), ServerConnectionCallbacks{
                [this](std::string &&message) { this->execute(std::move(message)); },
                {}
        });
        forDocumentConnection_->start();
        forDocumentConnection_->write(std::move(message));
    }
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
