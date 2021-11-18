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

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    scene = new PaintScene(this);       // Инициализируем графическую сцену
    qGraphicsView = new QGraphicsView;
    qGraphicsView->setScene(scene);
    setCentralWidget(qGraphicsView);

    // MenuBar
    auto *file_open = new QAction("&Open", this);
    auto *file_close = new QAction("&Close", this);
    auto *file_save = new QAction("&Save", this);
    auto *file_save_as = new QAction("&Save as", this);
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
//    parametersPanel = new ParametersPanel;
//    addToolBar(parametersPanel);
    parameters_panel = new QToolBar;
    addToolBar(parameters_panel);
    parameters_panel->setFixedHeight(40);

    auto *right_panel = new QToolBar;
    addToolBar(Qt::RightToolBarArea, right_panel);
    right_panel->setFixedWidth(400);

    toolsPanel = new ToolsPanel;
    addToolBar(Qt::LeftToolBarArea, toolsPanel);

    connect(toolsPanel->actions()[0], &QAction::triggered, this, &MainWindow::slotBrush);
    connect(this, &MainWindow::TemporarySignal, scene, &PaintScene::PaintCurveSlot);
    //connect(scene, &PaintScene::PushCurve, this, &MainWindow::TemporaryWriterSlot);


    //  Status bar
    statusBar()->showMessage("Status bar");

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(500);
}

MainWindow::~MainWindow() {
}

void MainWindow::slotTimer() {
    scene->setSceneRect(0, 0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);
}

void MainWindow::slotBrush() {

    if (scene->BrushStatus()) {
        parameters_panel->clear();
        //parametersPanel->clear();
    } else {
        //parametersPanel->setBrush();

        scene->SetBrush(30, 0, 180, 40, 255);
        scene->setSceneRect(0, 0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);

        auto *dummy = new QWidget();
        dummy->setFixedWidth(40);
        parameters_panel->addWidget(dummy);

        auto *size_changer = new Changer("Size", 30, 500, this);
        connect(size_changer, QOverload<int>::of(&Changer::valueChanged), scene, &PaintScene::SetBrushSize);
        parameters_panel->addWidget(size_changer);
        parameters_panel->addSeparator();


        auto *palette = new Palette;
        palette->SetColor(0, 180, 40, 255);

        auto *opacity_changer = new Changer("Opacity", 255, 255, this);
        connect(opacity_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetTransparencySlot));


        auto *red_changer = new Changer("Red", 0, 255, this);
        connect(red_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetRedSlot));
        connect(red_changer, QOverload<int>::of(&Changer::valueChanged), palette,
                (&Palette::SetRed));
        parameters_panel->addWidget(red_changer);

        auto *green_changer = new Changer("Green", 180, 255, this);
        connect(green_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetGreenSlot));
        connect(green_changer, QOverload<int>::of(&Changer::valueChanged), palette,
                (&Palette::SetGreen));
        parameters_panel->addWidget(green_changer);

        auto *blue_changer = new Changer("Blue", 40, 255, this);
        connect(blue_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetBlueSlot));
        connect(blue_changer, QOverload<int>::of(&Changer::valueChanged), palette,
                (&Palette::SetBlue));
        parameters_panel->addWidget(blue_changer);


//        Palette palette;
        parameters_panel->addWidget(palette);
        parameters_panel->addSeparator();

        parameters_panel->addWidget(opacity_changer);


        parameters_panel->setStyleSheet(QString("QToolBar {spacing: %1}").arg(10));
    }
    scene->ChangeBrushStatus();
}

void MainWindow::executeBrush(const Curve& curve) {
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

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
