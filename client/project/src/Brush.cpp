//
// Created by eremey on 27.11.2021.
//
#include "Brush.h"
#include <QIcon>
#include <QPixmap>

#include "CommandBus.h"
#include "Commands.h"
#include <cassert>

Brush::Brush(qreal brushSize, const QColor &brushColor, QWidget *parent) : brushSize(brushSize), brushColor(brushColor), QAction(parent) {
    assert(parent != nullptr);
    connect(this, &QAction::triggered, this, &Brush::BrushTriggeredSlot);

    QPixmap brush_pm("brush.png");
    setIcon(QIcon(brush_pm));
}
void Brush::BrushTriggeredSlot() {
    emit(BrushTriggered(brushSize, brushColor));
}
void Brush::SetSizeSlot(int value) {
    brushSize = value;
}

void Brush::SetRedSlot(int value) {
    brushColor.setRed(value);
}
void Brush::SetGreenSlot(int value) {
    brushColor.setGreen(value);
}
void Brush::SetBlueSlot(int value) {
    brushColor.setBlue(value);
}
void Brush::SetOpacitySlot(int value) {
    brushColor.setAlpha(value);
}
