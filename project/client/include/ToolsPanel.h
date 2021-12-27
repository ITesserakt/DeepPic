//
// Created by eremey on 18.11.2021.
//
#pragma once
#include <QToolBar>
#include "Palette.h"

class ToolsPanel : public QToolBar {
    Q_OBJECT

public:
    explicit ToolsPanel(QWidget *parent = nullptr);
    ~ToolsPanel() = default;

signals:
    void toolTriggered(char tool, size_t size, const QColor& color);

public slots:
    void SetBrushSizeSlot(int value);
    void SetBrushRedSlot(int value);
    void SetBrushGreenSlot(int value);
    void SetBrushBlueSlot(int value);
    void SetBrushOpacitySlot(int value);

    void unlockTools();
    //void lockTools();

private slots:
    void BrushTriggeredSlot(qreal brushSize, const QColor& brushColor);//
    void BrushTriggered();
    void LineTriggered();
    void RectangleTriggered();
    void CircleTriggered();
    void TextTriggered();
private:
signals:
    void BrushTriggeredS(qreal brushSize, const QColor& brushColor);

    void SetBrushSize(int value);
    void SetBrushRed(int value);
    void SetBrushGreen(int value);
    void SetBrushBlue(int value);
    void SetBrushOpacity(int value);

private:
    QAction *brush;
    QAction *line;
    QAction *rectangle;
    QAction *circle;


    Palette *palette;
    size_t size;
};

