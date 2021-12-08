//
// Created by eremey on 18.11.2021.
//
#pragma once
#include <QToolBar>
#include "Changer.h"
#include "Palette.h"
#include <QWidget>

class ParametersPanel : public QToolBar {
    Q_OBJECT

public:
    explicit ParametersPanel(QWidget *parent = 0);
    ~ParametersPanel() = default;

signals:
    void BrushSizeChanged(int newSize);
    void BrushRedChanged(int newValue);
    void BrushGreenChanged(int newValue);
    void BrushBlueChanged(int newValue);
    void BrushOpacityChanged(int newValue);
public slots:
    void setBrush(qreal brushSize = 10, const QColor& brushColor = Qt::red);

private:
    Changer* changers;

private slots:
    void BrushSizeChangedSlot(int newSize);
    void BrushRedChangedSlot(int newValue);
    void BrushGreenChangedSlot(int newValue);
    void BrushBlueChangedSlot(int newValue);
    void BrushOpacityChangedSlot(int newValue);

};

