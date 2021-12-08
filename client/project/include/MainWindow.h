#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QWidget>

#include "Canvas.h"
#include "PaintScene.h"
#include "ParametersPanel.h"
#include "ToolsPanel.h"
#include "CommandExecutor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(PaintScene &scene);
    ~MainWindow() = default;

signals:
    void TemporarySignal(const Curve &curve);///

private:
    PaintScene *scene;
    Canvas *canvas;

    QToolBar *parameters_panel;

    ToolsPanel *toolsPanel;

    ParametersPanel *parametersPanel;

    QTimer *timer;

private:
    long temporary_read_position = 0;

private slots:
    void slotBrush(qreal brushSize = 10, const QColor &brushColor = Qt::red);
    void slotTimer();
};
