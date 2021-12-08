#include <QPainter>
#include <QGraphicsPixmapItem>

#include "PaintScene.h"
#include "CommandBus.h"
#include "Commands.h"

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        command::BrushStarted cmd{event->scenePos(), brush_size, brush_color};
        bus.execute(cmd);
        previous_point = event->scenePos();
        line.emplace_back(previous_point);
    }
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        command::BrushMoved cmd{previous_point, event->scenePos(), brush_size, brush_color};
        bus.execute(cmd);
        previous_point = event->scenePos();
        line.push_back(previous_point);
    }
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    // TODO: write
    //    emit PushCurve({brush_size, brush_color.red(), brush_color.green(), brush_color.blue(),
    //               line});
    //    line.clear();
}
void PaintScene::ChangeBrushStatus() {
    is_brush = !is_brush;
}
PaintScene::PaintScene(CommandBus<true> &bus, QObject *parent) : bus(bus) {
    brush_color.setRgb(0, 255, 0);
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
void PaintScene::SetBrush(qreal brushSize, const QColor& brushColor) {
    brush_size = brushSize;
    brush_color = brushColor;
}
void PaintScene::PaintCurveSlot(const Curve &curve) {
    // TODO: read
    assert(curve.coords.size() > 1);
    assert(curve.color_red < 256 && curve.color_red >= 0);
    assert(curve.color_green < 256 && curve.color_green >= 0);
    assert(curve.color_blue < 256 && curve.color_blue >= 0);

    for (int i = 1; i < curve.coords.size(); ++i) {
//        std::cout << curve.coords[i].x() << "\n";
        addLine(curve.coords[i].x(),
                curve.coords[i].y() ,
                curve.coords[i - 1].x(),
                curve.coords[i - 1].y(),
                QPen(QColor(curve.color_red, curve.color_green, curve.color_blue),
                     curve.brush_size, Qt::SolidLine, Qt::RoundCap));
    }
}
