//
// Created by eremey on 27.11.2021.
//
#pragma once
#include <QAction>

class Brush : public QAction {
    Q_OBJECT

public:
    explicit Brush(qreal brushSize = 10, QWidget *parent = nullptr);
    ~Brush() = default;

signals:
    void BrushTriggered(qreal brushSize, const QColor& brushColor);

public slots:
    void SetSizeSlot(int value);
    void SetRedSlot(int value);
    void SetGreenSlot(int value);
    void SetBlueSlot(int value);
    void SetOpacitySlot(int value);
private slots:
    void BrushTriggeredSlot();
private:
    qreal brushSize;
    QColor brushColor;
};