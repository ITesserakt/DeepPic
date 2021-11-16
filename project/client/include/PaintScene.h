#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QColor>

struct point {
    int32_t i = 0;
    int32_t j = 0;
};

struct Curve {
    int32_t brush_size = 0;
    int32_t color_red = 0;
    int32_t color_green = 0;
    int32_t color_blue = 0;

    std::vector<point> coords;
};

class PaintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit PaintScene(QObject *parent = 0);
    ~PaintScene() = default;

    void ChangeBrushStatus();
    bool BrushStatus();
    void SetBrushSize(qreal brush_size = 10);
    void SetBrush(qreal size, int red, int green, int blue, int opacity);

public slots:
    void SetBrushSizeSlot(int size);
    void SetTransparencySlot(int transparency);
    void SetRedSlot(int transparency);
    void SetGreenSlot(int transparency);
    void SetBlueSlot(int transparency);

    void PaintCurveSlot(const Curve& curve);

private:
    bool is_brush = false;

    QPointF previous_point;

    std::vector<QPointF> line;

    qreal brush_size = 10;

    QColor brush_color;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};
