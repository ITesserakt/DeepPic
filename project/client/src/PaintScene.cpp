#include "PaintScene.h"
#include <iostream>

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        addEllipse(event->scenePos().x() - brush_size / 2,
                   event->scenePos().y() - brush_size / 2,
                   brush_size,
                   brush_size,
                   QPen(Qt::NoPen),
                   QBrush(brush_color));
        previous_point = event->scenePos();
        line.push_back(previous_point);
    }

}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        addLine(previous_point.x(),
                previous_point.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(brush_color, brush_size, Qt::SolidLine, Qt::RoundCap));
        previous_point = event->scenePos();
        line.push_back(previous_point);
    }
}


void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << line.size() << std::endl;
    line.clear();
}
void PaintScene::ChangeBrushStatus() {
    is_brush = !is_brush;
}
PaintScene::PaintScene(QObject *parent) {
    brush_color.setRgb(0,255,0);

}
void PaintScene::SetBrushSize(qreal brush_size) {
    this->brush_size = brush_size;
}
bool PaintScene::BrushStatus() {
    return is_brush;
}
void PaintScene::SetBrushSizeSlot(int size) {
    brush_size = size;
}
void PaintScene::SetTransparencySlot(int value) {
    assert(value < 256 && value >= 0);
    brush_color.setAlpha(value);
}
void PaintScene::SetRedSlot(int value) {
    assert(value < 256 && value >= 0);
    brush_color.setRed(value);
}

void PaintScene::SetGreenSlot(int value) {
    assert(value < 256 && value >= 0);
    brush_color.setGreen(value);
}

void PaintScene::SetBlueSlot(int value) {
    assert(value < 256 && value >= 0);
    brush_color.setBlue(value);
}
void PaintScene::SetBrush(qreal size, int red, int green, int blue, int opacity) {
    brush_size = size;
    brush_color.setRgb(red, green, blue, opacity);
}
void PaintScene::PaintCurveSlot(const Curve &curve) {
    assert(!curve.coords.empty());
    assert(curve.color_red < 256 && curve.color_red >= 0);
    assert(curve.color_green < 256 && curve.color_green >= 0);
    assert(curve.color_blue < 256 && curve.color_blue >= 0);

    for (int i = 0; i < curve.coords.size() - 2; ++i) {
        addLine(curve.coords[i].i,
                curve.coords[i].j,
                curve.coords[i + 1].i,
                curve.coords[i + 1].j,
                QPen(QColor(curve.color_red, curve.color_green, curve.color_blue),
                     curve.brush_size, Qt::SolidLine, Qt::RoundCap));
    }
}
