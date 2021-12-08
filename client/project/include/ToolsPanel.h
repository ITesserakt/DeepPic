//
// Created by eremey on 18.11.2021.
//
#pragma once
#include <QToolBar>

class ToolsPanel : public QToolBar {
    Q_OBJECT

public:
    explicit ToolsPanel(QWidget *parent = nullptr);
    ~ToolsPanel() = default;

public slots:
    void SetBrushSizeSlot(int value);
    void SetBrushRedSlot(int value);
    void SetBrushGreenSlot(int value);
    void SetBrushBlueSlot(int value);
    void SetBrushOpacitySlot(int value);

private slots:
    void BrushTriggeredSlot(qreal brushSize, const QColor& brushColor);
private:
signals:
    void BrushTriggered(qreal brushSize, const QColor& brushColor);

    void SetBrushSize(int value);
    void SetBrushRed(int value);
    void SetBrushGreen(int value);
    void SetBrushBlue(int value);
    void SetBrushOpacity(int value);
};

