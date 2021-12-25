//
// Created by eremey on 18.11.2021.
//

#include <QAction>
#include <QPixmap>
#include "ToolsPanel.h"

#include "Brush.h"


ToolsPanel::ToolsPanel(QWidget *parent) : QToolBar(parent), size(20) {
    QString hardPath = "../../../icons/"; // <-bad practice
    QPixmap brush_pm(hardPath + "brush.png");
    brush_pm.load(hardPath + "line.png");

//    auto *brush = new Brush(20, this);
//    addAction(brush);

    brush_pm.load(hardPath + "brush.png");
    QAction *brush = addAction(brush_pm, "B");
    connect(brush, &QAction::triggered, this, &ToolsPanel::BrushTriggered);

    brush_pm.load(hardPath + "line.png");
    QAction *line = addAction(brush_pm, "L");
    connect(line, &QAction::triggered, this, &ToolsPanel::LineTriggered);

    brush_pm.load(hardPath + "rectangle.png");
    QAction *rectangle = addAction(brush_pm, "R");
    connect(rectangle, &QAction::triggered, this, &ToolsPanel::RectangleTriggered);

    brush_pm.load(hardPath + "circle.png");
    QAction *circle = addAction(brush_pm, "C");
    connect(circle, &QAction::triggered, this, &ToolsPanel::CircleTriggered);

    brush_pm.load(hardPath + "text.png");
    QAction *text = addAction(brush_pm, "T");
    connect(text, &QAction::triggered, this, &ToolsPanel::TextTriggered);

    QAction *space = addAction("");
    text->setEnabled(false);

//    connect(brush, &Brush::BrushTriggeredS, this, &ToolsPanel::BrushTriggeredSlot);
//
//    connect(this, &ToolsPanel::SetBrushSize, brush, &Brush::SetSizeSlot);
//    connect(this, &ToolsPanel::SetBrushRed, brush, &Brush::SetRedSlot);
//    connect(this, &ToolsPanel::SetBrushGreen, brush, &Brush::SetGreenSlot);
//    connect(this, &ToolsPanel::SetBrushBlue, brush, &Brush::SetBlueSlot);
//    connect(this, &ToolsPanel::SetBrushOpacity, brush, &Brush::SetOpacitySlot);

    palette = new Palette(this);
    palette->setFixedSize(30,30);
    palette->SetColor(0, 150, 0);
    addSeparator();
    addWidget(palette);
    addSeparator();
}
void ToolsPanel::BrushTriggeredSlot(qreal brushSize, const QColor& brushColor) {
    emit(BrushTriggeredS(brushSize, brushColor));
}
void ToolsPanel::SetBrushSizeSlot(int value) {
    size = value;
}
void ToolsPanel::SetBrushRedSlot(int value) {
    palette->setRed(value);
}
void ToolsPanel::SetBrushGreenSlot(int value) {
    palette->setGreen(value);
}
void ToolsPanel::SetBrushBlueSlot(int value) {
    palette->setBlue(value);
}
void ToolsPanel::SetBrushOpacitySlot(int value) {
    palette->setOpacity(value);
}

void ToolsPanel::BrushTriggered() {
    emit(toolTriggered('B', size, palette->getColor()));
}
void ToolsPanel::LineTriggered() {
    emit(toolTriggered('L', size, palette->getColor()));
}
void ToolsPanel::RectangleTriggered() {
    emit(toolTriggered('R', size, palette->getColor()));
}
void ToolsPanel::CircleTriggered() {
    emit(toolTriggered('C', size, palette->getColor()));
}
void ToolsPanel::TextTriggered() {
    emit(toolTriggered('T', size, palette->getColor()));
}
