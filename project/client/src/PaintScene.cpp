#include "PaintScene.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <sstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        addEllipse(event->scenePos().x() - brush_size / 2,
                   event->scenePos().y() - brush_size / 2,
                   brush_size,
                   brush_size,
                   QPen(Qt::NoPen),
                   QBrush(brush_color));
        previous_point = event->scenePos().toPoint();
        curve.push_back(previous_point);
    }

}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (is_brush) {
        addLine(previous_point.x(),
                previous_point.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(brush_color, brush_size, Qt::SolidLine, Qt::RoundCap));
        previous_point = event->scenePos().toPoint();
        curve.push_back(previous_point);
    }
}


void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    char separator = ' ';
    std::string message = std::to_string(brush_size) + separator
                          + std::to_string(brush_color.red()) + separator
                          + std::to_string(brush_color.green()) + separator
                          + std::to_string(brush_color.blue()) + separator
                          + std::to_string(brush_color.alpha()) + separator;


    for (auto& point: curve) {
        message +=  std::to_string(point.x()) + separator + std::to_string(point.y()) + separator;
    }

    // TODO: 1) message -> command
    json command_json = {{"target", "sharing_command"}, {"command", message}};
    std::string command_str = command_json.dump();

    emit(writeCurveSignal(command_str));

    curve.clear();
}
void PaintScene::ChangeBrushStatus() {
    is_brush = !is_brush;
}
PaintScene::PaintScene(QObject *parent) {
    //connect(this, &PaintScene::writeCurveSignal, this, &PaintScene::readCurveSlot); //
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
void PaintScene::SetBrush(qreal brushSize, const QColor& brushColor) {
    brush_size = brushSize;
    brush_color = brushColor;
}

void PaintScene::readCurveSlot(std::string &message) {
    std::stringstream curveStr;
    curveStr << message;   // bad idea

    Curve brushCurve;
    curveStr >> brushCurve.brush_size >> brushCurve.color_red >> brushCurve.color_green >> brushCurve.color_blue >> brushCurve.opacity;
    int32_t point_x = 0;
    int32_t point_y = 0;

    while (curveStr >> point_x >> point_y){
        brushCurve.coords.emplace_back(point_x, point_y);
    }

    assert(brushCurve.coords.size() > 1);
    assert(brushCurve.color_red < 256 && brushCurve.color_red >= 0);
    assert(brushCurve.color_green < 256 && brushCurve.color_green >= 0);
    assert(brushCurve.color_blue < 256 && brushCurve.color_blue >= 0);

    for (int i = 1; i < brushCurve.coords.size(); ++i) {
        addLine(brushCurve.coords[i].x(),
                brushCurve.coords[i].y() ,
                brushCurve.coords[i - 1].x(),
                brushCurve.coords[i - 1].y(),
                QPen(QColor(brushCurve.color_red, brushCurve.color_green, brushCurve.color_blue, brushCurve.opacity),
                     brushCurve.brush_size, Qt::SolidLine, Qt::RoundCap));
    }
}
