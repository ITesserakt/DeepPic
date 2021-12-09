#pragma once

#include <QColor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include "CommandBus.h"

struct Point {
    int32_t i = 0;
    int32_t j = 0;
};

struct Curve {
    double brush_size = 0;
    int32_t color_red = 0;
    int32_t color_green = 0;
    int32_t color_blue = 0;

    std::vector<QPointF> coords;
};

class PaintScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit PaintScene(CommandBus &bus, QObject *parent = 0);
    ~PaintScene() = default;

    void ChangeBrushStatus();
    bool BrushStatus();
    void SetBrushSize(qreal brush_size = 10);
    void SetBrush(qreal brushSize = 10, const QColor &brushColor = Qt::red);

signals:
    void PushCurve(const Curve &curve);

public slots:
    void SetBrushSizeSlot(int size);
    void SetTransparencySlot(int transparency);
    void SetRedSlot(int transparency);
    void SetGreenSlot(int transparency);
    void SetBlueSlot(int transparency);

    void PaintCurveSlot(const Curve &curve);

private:
    bool is_brush = false;
    QPointF previous_point;
    std::vector<QPointF> line;
    qreal brush_size = 10;
    QColor brush_color;
    CommandBus &bus;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
