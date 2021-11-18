#pragma once
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMainWindow>
#include <QPushButton>

#include "PaintScene.h"
#include "ToolsPanel.h"
#include "ParametersPanel.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void TemporarySignal(const Curve& curve); ///

private:
    PaintScene *scene;
    QGraphicsView* qGraphicsView;

    QToolBar *parameters_panel;

    ToolsPanel *toolsPanel;

    ParametersPanel *parametersPanel;

    QTimer *timer;

private:
    long temporary_read_position = 0;

private slots:
    void slotBrush();
    void slotTimer();

public:
    void executeBrush(const Curve& curve);
};
