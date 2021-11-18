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
    QAction *file_open = new QAction("&Open", this);
    QAction *file_close = new QAction("&Close", this);
    QAction *file_save = new QAction("&Save", this);
    QAction *file_save_as = new QAction("&Save as", this);
    QMenu *file;
    file = menuBar()->addMenu("File");
    file->addAction(file_open);
    file->addAction(file_close);
    file->addAction(file_save);
    file->addAction(file_save_as);

    QAction *edit_undo = new QAction("&Undo", this);
    QAction *edit_redo = new QAction("&Redo", this);
    QMenu *edit;
    edit = menuBar()->addMenu("Edit");
    edit->addAction(edit_undo);
    edit->addAction(edit_redo);

    QAction *help_help = new QAction("&Help", this);
    QMenu *help;
    help = menuBar()->addMenu("Help");
    help->addAction(help_help);

    // ToolBars
    parameters_panel = new QToolBar;
    addToolBar(parameters_panel);
    parameters_panel->setFixedHeight(40);

    QToolBar *right_panel = new QToolBar;
    addToolBar(Qt::RightToolBarArea, right_panel);
    right_panel->setFixedWidth(400);


    QPixmap brush_pm("brush.png");
    //QAction *brush = new QAction(QIcon(brush_pm), "&Brush", this);
    QToolBar *tools_panel = new QToolBar;
    addToolBar(Qt::LeftToolBarArea, tools_panel);
    QAction *brush = tools_panel->addAction(QIcon(brush_pm), "Brush");
    tools_panel->addAction(brush);
    connect(brush, &QAction::triggered, this, &MainWindow::slotBrush);
    connect(this, &MainWindow::TemporarySignal, scene, &PaintScene::PaintCurveSlot);
    connect(scene, &PaintScene::PushCurve, this, &MainWindow::TemporaryWriterSlot);


    //  Status bar
    statusBar()->showMessage("Status bar");

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(500);
}

MainWindow::~MainWindow() {
//    free(qGraphicsView);
}

void MainWindow::slotTimer() {
    scene->setSceneRect(0, 0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);
    std::ifstream isf ("House.txt");
    Curve curve;
    int i = 0;
    while (i < 1) {
        isf >> curve.brush_size >> curve.color_red >> curve.color_green >> curve.color_blue;

        size_t curve_size = 0;
        isf >> curve_size;

        QPointF point;
        qreal point_x = 0;
        qreal point_y = 0;

        for (size_t i = 0; i < curve_size; ++i) {

            isf >> point_x >> point_y;
            point.setX(point_x);
            point.setY(point_y);

            curve.coords.push_back(point);
            //std::cout << point.j << " " << point.i << std::endl;
        }
        temporary_read_position = isf.tellg();
        emit(TemporarySignal(curve));
        i++;
    }
//    isf >> curve.brush_size >> curve.color_red >> curve.color_green >> curve.color_blue;
//
//    size_t curve_size = 0;
//    isf >> curve_size;
//
//    Point point;
//    for (size_t i = 0; i < curve_size; ++i) {
//        isf >> point.i >> point.j;
//        curve.coords.push_back(point);
//    }
//    temporary_read_position = isf.tellg();

    isf.close();


    std::cout << "slotTimer\n";
    timer->stop();
    emit(TemporarySignal(curve));
}

void MainWindow::slotBrush() {
    if (scene->BrushStatus()) {
        parameters_panel->clear();
    } else {
        scene->SetBrush(30, 0, 180, 40, 255);
        scene->setSceneRect(0, 0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);

        QWidget *dummy = new QWidget();
        dummy->setFixedWidth(40);
        parameters_panel->addWidget(dummy);

        Changer *size_changer = new Changer("Size", 30, 500, this);
        connect(size_changer, QOverload<int>::of(&Changer::valueChanged), scene, &PaintScene::SetBrushSize);
        parameters_panel->addWidget(size_changer);
        parameters_panel->addSeparator();


        Palette *palette = new Palette;
        palette->SetColor(0, 180, 40, 255);

        Changer *opacity_changer = new Changer("Opacity", 255, 255, this);
        connect(opacity_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetTransparencySlot));


        Changer *red_changer = new Changer("Red", 0, 255, this);
        connect(red_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetRedSlot));
        connect(red_changer, QOverload<int>::of(&Changer::valueChanged), palette,
                (&Palette::SetRed));
        parameters_panel->addWidget(red_changer);

        Changer *green_changer = new Changer("Green", 180, 255, this);
        connect(green_changer, QOverload<int>::of(&Changer::valueChanged), scene,
                (&PaintScene::SetGreenSlot));
        connect(green_changer, QOverload<int>::of(&Changer::valueChanged), palette,
                (&Palette::SetGreen));
        parameters_panel->addWidget(green_changer);

        Changer *blue_changer = new Changer("Blue", 40, 255, this);
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
void MainWindow::TemporaryWriterSlot(const Curve &curve) {

    std::ofstream outf("House.txt", std::ios::app);

    outf << curve.brush_size << " " << curve.color_red << " " << curve.color_green << " " << curve.color_blue << " " <<
            curve.coords.size() << std::endl;
    for (auto& point: curve.coords) {
        outf << point.x() << " " << point.y() << " ";
    }
    outf << std::endl;

    outf.close();
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
