#pragma once
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMainWindow>
#include <QPushButton>

#include "PaintScene.h"
#include "ToolsPanel.h"
#include "ParametersPanel.h"
#include "Canvas.h"

#include "ServerConnection.h"

#include "Connector.h"


class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    PaintScene *scene;
    Canvas* canvas;

    QToolBar *parameters_panel;

    ToolsPanel *toolsPanel;

    ParametersPanel *parametersPanel;

    Connector *connector;

    QTimer *timer;

    std::unique_ptr<ServerConnection> serverConnection_;
    std::unique_ptr<ServerConnection> forDocumentConnection_;

signals:
    void failedShareSignal();
    void successfulShareSignal(const QString& message);

    void failedConnectSignal();
    void successfulConnectSignal(const QString& message);

    void addCurve(const QString& message);
    void setImageSignal(const QVector<unsigned char> imageVector);

    void saveImageToSignal(const QString& path);

private:
    long temporary_read_position = 0;

private slots:
    void slotBrush(qreal brushSize = 10, const QColor& brushColor = Qt::red);
    void slotTimer();

//    void

public slots:
    void writeSlot(std::string& message);


public:
    void execute(std::string&& message);
    //void executeBrush(const Curve& curve);
};
