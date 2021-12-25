#include "MainWindow.h"
#include "QGraphicsView"
#include "Connector.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QStatusBar>
#include <QToolBar>

#include <QFileDialog>

#include <stdexcept>

#include <QImage>

#include <iostream>

#include <nlohmann/json.hpp>
#include <fstream>

#include "Base64.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent) {
    setFixedSize(1000, 800);

    scene = new PaintScene(this);
    canvas = new Canvas(this);
    canvas->setScene(scene);

    setCentralWidget(canvas);

    connect(scene, &PaintScene::writeCurveSignal, this, &MainWindow::writeSlot);
    connect(this, &MainWindow::addCurve, scene, &PaintScene::execute);

    // MenuBar
    auto *file_open = new QAction("&Open", this);
    connect(file_open, &QAction::triggered, this, &MainWindow::loadImage);
    auto *file_save = new QAction("&Save", this);
    connect(file_save, &QAction::triggered, this, &MainWindow::saveImage);
    auto *file_save_as = new QAction("&Save as", this);
    connect(file_save_as, &QAction::triggered, this, &MainWindow::saveAsImage);
    QMenu *file;
    file = menuBar()->addMenu("File");
    file->addAction(file_open);
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
    parametersPanel->setFixedHeight(40);

    connector = new Connector(this);
    addToolBar(Qt::RightToolBarArea, connector);
    connector->setFixedWidth(400);

    connect(connector, &Connector::writeSignal, this, &MainWindow::writeSlot);
    connect(this, &MainWindow::failedShareSignal, connector, &Connector::failedShareSlot);
    connect(this, &MainWindow::failedConnectSignal, connector, &Connector::failedConnectSlot);
    connect(this, &MainWindow::successfulConnectSignal, connector, &Connector::successfulConnectSlot);
    connect(this, &MainWindow::successfulShareSignal, connector, &Connector::successfulShareSlot);

    toolsPanel = new ToolsPanel(this);
    addToolBar(Qt::LeftToolBarArea, toolsPanel);

    connect(toolsPanel, &ToolsPanel::BrushTriggered, this, &MainWindow::slotBrush);

    //  Status bar
    statusBar()->showMessage("Status bar");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(500);
    serverConnection_ = std::make_unique<ServerConnection>(std::string("127.0.0.1"), 8080, ServerConnectionCallbacks{
            [this](std::string &&message) { this->execute(std::move(message)); },
            {}
    });

    serverConnection_->start();

}

MainWindow::~MainWindow() {}

void MainWindow::slotTimer() {
    scene->setSceneRect(0, 0, canvas->width() - 16, canvas->height() - 16);
    timer->stop();
    canvas->update();
}

void MainWindow::slotBrush(qreal brushSize, const QColor &brushColor) {

    if (scene->BrushStatus()) {
        parametersPanel->clear();
    } else {
        scene->SetBrush(brushSize, brushColor);
        parametersPanel->setBrush(brushSize, brushColor);
        connect(parametersPanel, &ParametersPanel::BrushSizeChanged, scene, &PaintScene::SetBrushSize);
        connect(parametersPanel, &ParametersPanel::BrushSizeChanged, toolsPanel, &ToolsPanel::SetBrushSizeSlot);

        connect(parametersPanel, &ParametersPanel::BrushRedChanged, scene, &PaintScene::SetRedSlot);
        connect(parametersPanel, &ParametersPanel::BrushRedChanged, toolsPanel, &ToolsPanel::SetBrushRedSlot);

        connect(parametersPanel, &ParametersPanel::BrushGreenChanged, scene, &PaintScene::SetGreenSlot);
        connect(parametersPanel, &ParametersPanel::BrushGreenChanged, toolsPanel, &ToolsPanel::SetBrushGreenSlot);

        connect(parametersPanel, &ParametersPanel::BrushBlueChanged, scene, &PaintScene::SetBlueSlot);
        connect(parametersPanel, &ParametersPanel::BrushBlueChanged, toolsPanel, &ToolsPanel::SetBrushBlueSlot);

        connect(parametersPanel, &ParametersPanel::BrushOpacityChanged, scene, &PaintScene::SetTransparencySlot);
        connect(parametersPanel, &ParametersPanel::BrushOpacityChanged, toolsPanel, &ToolsPanel::SetBrushOpacitySlot);
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
                to_string(parse_message["address"]) + std::string("|") + to_string(parse_message["port"]) + std::string("|") +
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
                to_string(parse_message["address"]) + std::string("|") + to_string(parse_message["port"]) + std::string("|") +
                to_string(parse_message["auth_token"]));
        std::cout << "// TODO: if joining succeeded" << std::endl;
        if (!is_author_document) {
            json json_msg = {{"target", "get_document"}};
            forDocumentConnection_->write(json_msg.dump());
        }
        emit(successfulConnectSignal(auth_token));
    } else

        // TODO: if need to draw a line
    if (parse_message["target"] == "sharing_command") {
        std::cout << "TODO: if need to draw a line" << std::endl;
        std::cout << "command: " << parse_message["command"] << std::endl;
        QString command = QString::fromStdString(parse_message["command"]);
//        std::string command = "20 0 128 0 255 402 243 400 248";
        emit(addCurve(command));
        //QString msg = "30";
//        emit(addTestPoint(msg));
    } else

        /*
        // TODO: if need get Image
        if (parse_message["target"] == "get_document" && parse_message["status"].is_null()) {
            std::vector<unsigned char> imageVector = canvas->GetImageVector().toStdVector();
            std::string base64_image = base64_encode(&imageVector.front(), imageVector.size());
            std::cout << "base64 image size = " << base64_image.size() << std::endl;
            json json_msg = {{"status", "OK"}, {"target", "get_document"}, {"document", std::move(base64_image)}};

            forDocumentConnection_->write(json_msg.dump());
        } else

        // TODO: if need set Image
        if (parse_message["target"] == "get_document" && !parse_message["status"].is_null() && parse_message["status"] == "OK") {
            std::vector<unsigned char> imageVector = base64_decode(parse_message["document"]); // <- for writing
            emit(setImageSignal(QVector<unsigned char>::fromStdVector(imageVector)));
        */

        // TODO: if need to save Image
    if (parse_message["target"] == "get_document" && parse_message["status"].is_null()) {
        saveImageTo("../data/canvas1.jpg"); // <- TODO: change path
        // получаем массив байт из изображения
        std::ifstream input("../data/canvas1.jpg", std::ios::binary);
        std::vector<unsigned char> image_vector(std::istreambuf_iterator<char>(input), {});
        std::string base64_image = base64_encode(&image_vector.front(), image_vector.size());
        json json_msg = {{"status",   "OK"},
                         {"target",   "get_document"},
                         {"document", std::move(base64_image)}};
        forDocumentConnection_->write(json_msg.dump());
        //json json_msg = {{"status", "OK"}, {"target", "get_document"}, {"document", "derzhi zhabu"}};

        //forDocumentConnection_->write(json_msg.dump());
    } else
        // TODO: if need to setImage Image
    if (parse_message["target"] == "get_document" && !parse_message["status"].is_null() && parse_message["status"] == "OK") {
        std::vector<unsigned char> image_vector = base64_decode(parse_message["document"]);
        std::ofstream outfile("../data/canvas2.jpg", std::ios::out | std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(image_vector.data()), image_vector.size());
        loadImageFrom("../data/canvas2.jpg"); // <- TODO: change path
    }
}


void MainWindow::writeSlot(std::string &message) {
    //std::string target = json::parse(message)["target"];
    auto json_message = json::parse(message);
    std::cerr << "MainWindow::writeSlot, message = " << message << std::endl;
    if (json_message["target"] == "sharing_document" && serverConnection_) {
        serverConnection_->write(std::move(message));
        is_author_document = true;
    } else if (forDocumentConnection_) {
        forDocumentConnection_->write(std::move(message));
    } else {
        is_author_document = false;
        std::string port = json_message["port"];
        forDocumentConnection_ = std::make_unique<ServerConnection>(json_message["address"], std::stoi(port), ServerConnectionCallbacks{
                [this](std::string &&message) { this->execute(std::move(message)); },
                {}
        });
        forDocumentConnection_->start();
        forDocumentConnection_->write(std::move(message));
    }
}

/// save/load logic
void MainWindow::loadImage() {
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    loadImageFrom(path);
}

void MainWindow::saveImage() {
    if (filePath == nullptr) {
        filePath = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), ".");
    }
    saveImageTo(filePath);
}

void MainWindow::saveAsImage() {
    filePath = QFileDialog::getSaveFileName(this,
                                            tr("Save Image"), ".");
    saveImageTo(filePath);
}

void MainWindow::loadImageFrom(const QString &path) {
    timer->start(500);
    auto *image = new QImage;
    image->load(path);

    if (image->height() < height()) {
        canvas->setFixedHeight(image->height() + 16);  // 16 - changerSlider height
    }
    if (image->width() < width()) {
        canvas->setFixedWidth(image->width() + 16);
    }

    canvas->setSceneRect(0, 0, image->width(), image->height());
    canvas->scene()->addPixmap(QPixmap::fromImage(*image));
    delete image;
}

void MainWindow::loadImageFrom(const std::vector <unsigned char> buf) {
    QByteArray img(reinterpret_cast<const char*>(buf.data()), buf.size());
    QPixmap pix1;
    pix1.loadFromData(img, "JPG");
    canvas->scene()->addPixmap(pix1.scaled(1000, 1000, Qt::KeepAspectRatio));
}

void MainWindow::saveImageTo(const QString &path) {
    QImage image(canvas->scene()->width(), canvas->scene()->height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    canvas->setRenderHint(QPainter::Antialiasing);
    canvas->render(&painter);
    image.save(path, "JPG");
}
///

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
