#pragma once
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMainWindow>
#include <QPushButton>

#include "PaintScene.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void TemporarySignal(const Curve& curve);

private:
    PaintScene *scene;
    QGraphicsView* qGraphicsView;

    QToolBar *parameters_panel;

private:

private slots:
    void slotBrush();
};
