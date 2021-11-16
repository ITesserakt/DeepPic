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


    //  Status bar
    statusBar()->showMessage("Status bar");

//    timer = new QTimer();
//    //connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
//    timer->start(100);
}

MainWindow::~MainWindow() {
//    free(qGraphicsView);
}

//void MainWindow::slotTimer()
//{
//    timer->stop();
//    scene->setSceneRect(0,0, qGraphicsView->width() - 20, qGraphicsView->height() - 20);
//}

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
    emit(TemporarySignal({20,100,100,100,{{0,0},{50,50}}}));
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    timer->start(100);
//    QWidget::resizeEvent(event);
//}
