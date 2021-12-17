//
// Created by eremey on 18.11.2021.
//

#include <QAction>
#include <QPixmap>
#include "ToolsPanel.h"

#include "Brush.h"

ToolsPanel::ToolsPanel(QWidget *parent) : QToolBar(parent) {
    QPixmap brush_pm("brush.png");

    auto *brush = new Brush(20, Qt::darkGreen, this);
    addAction(brush);

    QPixmap pixmap(brush_pm.size());
    pixmap.fill( Qt::white);

    QAction *lasso = addAction(pixmap, "L");
    QAction *elaser = addAction(pixmap, "E");
    QAction *filler = addAction(pixmap, "F");
    QAction *scaling = addAction(pixmap, "S");
    QAction *loupe = addAction(pixmap, "L");
    QAction *primitives = addAction(pixmap, "P");

    connect(brush, &Brush::BrushTriggered, this, &ToolsPanel::BrushTriggeredSlot);

    connect(this, &ToolsPanel::SetBrushSize, brush, &Brush::SetSizeSlot);
    connect(this, &ToolsPanel::SetBrushRed, brush, &Brush::SetRedSlot);
    connect(this, &ToolsPanel::SetBrushGreen, brush, &Brush::SetGreenSlot);
    connect(this, &ToolsPanel::SetBrushBlue, brush, &Brush::SetBlueSlot);
    connect(this, &ToolsPanel::SetBrushOpacity, brush, &Brush::SetOpacitySlot);
}
void ToolsPanel::BrushTriggeredSlot(qreal brushSize, const QColor& brushColor) {
    emit(BrushTriggered(brushSize, brushColor));
}
void ToolsPanel::SetBrushSizeSlot(int value) {
    emit(SetBrushSize(value));
}
void ToolsPanel::SetBrushRedSlot(int value) {
    emit(SetBrushRed(value));
}
void ToolsPanel::SetBrushGreenSlot(int value) {
    emit(SetBrushGreen(value));
}
void ToolsPanel::SetBrushBlueSlot(int value) {
    emit(SetBrushBlue(value));
}
void ToolsPanel::SetBrushOpacitySlot(int value) {
    emit(SetBrushOpacity(value));
}
