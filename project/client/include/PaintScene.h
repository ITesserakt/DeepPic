#pragma once

#include <QGraphicsScene>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QColor>

struct Point {
    int32_t i = 0;
    int32_t j = 0;
};

struct Curve {
    int32_t brush_size = 0;
    int32_t color_red = 0;
    int32_t color_green = 0;
    int32_t color_blue = 0;
    int32_t opacity = 0;

    std::vector<QPointF> coords;
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
    void SetBrush(qreal brushSize = 10, const QColor& brushColor = Qt::red);

signals:
//    void PushCurve(const Curve& curve);
    void writeCurveSignal(std::string& curve);

public slots:
    void SetBrushSizeSlot(int size);
    void SetTransparencySlot(int transparency);
    void SetRedSlot(int transparency);
    void SetGreenSlot(int transparency);
    void SetBlueSlot(int transparency);

//    void PaintCurveSlot(const Curve& curve);

    void readCurveSlot(std::string& message);

private:
    bool is_brush = false;

    QPoint previous_point;

    std::vector<QPoint> curve;

    int32_t brush_size = 10;

    QColor brush_color;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};
