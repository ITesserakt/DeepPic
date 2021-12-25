#include "PaintScene.h"
#include "Base64.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <sstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void PaintScene::addPointToCommand(QPoint& point) {
    union {
        unsigned char bytes[4];
        int16_t coord[2] = {0, 0}; // 0 - x, 1 - y
    } pointUnion;
    pointUnion.coord[0] = point.x();
    pointUnion.coord[1] = point.y();

    command.push_back(pointUnion.bytes[0]);
    command.push_back(pointUnion.bytes[1]);
    command.push_back(pointUnion.bytes[2]);
    command.push_back(pointUnion.bytes[3]);
}



void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool == 'B') {
        addEllipse(event->scenePos().x() - currentSize / 2,
                   event->scenePos().y() - currentSize / 2,
                   currentSize,
                   currentSize,
                   QPen(Qt::NoPen),
                   QBrush(currentColor));
        previous_point = event->scenePos().toPoint();
        command[0] = 'B';
        addPointToCommand(previous_point);
    }
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool == 'B') {
        addLine(previous_point.x(),
                previous_point.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(currentColor, currentSize, Qt::SolidLine, Qt::RoundCap));
        previous_point = event->scenePos().toPoint();
        addPointToCommand(previous_point);
    }


}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (currentTool == 'B') {
        char separator = ' ';

        std::basic_string<char> encodedCommand = base64_encode(&command.front(), command.size());
        json command_json = {{"target", "sharing_command"}, {"command", encodedCommand}};
        std::string command_str = command_json.dump();

        emit(writeCurveSignal(command_str));

        command.resize(7); // clear command
    }
}

void PaintScene::ChangeBrushStatus() {
    if (currentTool != 'E') {
        currentTool = 'E';
    }
}
PaintScene::PaintScene(QObject *parent) : currentTool('E'), currentSize(10), currentColor(0, 255, 0) {

    command.resize(7);
    command[0] = 0; // command type
    command[1] = 10; //
    command[2] = 0;  //  size
    command[3] = currentColor.red();  //
    command[4] = currentColor.green();  //
    command[5] = currentColor.blue();  //
    command[6] = 255;  //
}
void PaintScene::SetBrushSize(qreal size) {
    currentSize = size;

    union {
        unsigned char bytes[2];
        int16_t value;
    } commandBrushSize;

    commandBrushSize.value = size;
    command[1] = commandBrushSize.bytes[0];
    command[2] = commandBrushSize.bytes[1];
    currentSize = size;
}
char PaintScene::getStatus() const{
    return currentTool;
}

void PaintScene::SetBrushSizeSlot(int size) {
    SetBrushSize(size);
}
void PaintScene::SetTransparencySlot(int value) {
    assert(value < 256 && value >= 0);
    currentColor.setAlpha(value);
    command[6] = currentColor.alpha();
}
void PaintScene::SetRedSlot(int value) {
    assert(value < 256 && value >= 0);
    currentColor.setRed(value);
    command[3] = currentColor.red();
}

void PaintScene::SetGreenSlot(int value) {
    assert(value < 256 && value >= 0);
    currentColor.setGreen(value);
    command[4] = currentColor.green();
}

void PaintScene::SetBlueSlot(int value) {
    assert(value < 256 && value >= 0);
    currentColor.setBlue(value);
    command[5] = currentColor.blue();
}
void PaintScene::SetBrush(qreal brushSize, const QColor& brushColor) {
    SetBrushSizeSlot(brushSize);
    //currentSize = brushSize;
    this->currentColor = brushColor;
    command[3] = brushColor.red();
    command[4] = brushColor.green();
    command[5] = brushColor.blue();
    command[6] = brushColor.alpha();
}

void PaintScene::readCurveSlot(const QString &message) {
    std::stringstream curveStr;
    curveStr << message.toStdString();   // bad idea

    Curve brushCurve;
    curveStr >> brushCurve.brush_size >> brushCurve.color_red >> brushCurve.color_green >> brushCurve.color_blue >> brushCurve.opacity;
    int32_t point_x = 0;
    int32_t point_y = 0;

    while (curveStr >> point_x >> point_y){
        brushCurve.coords.emplace_back(point_x, point_y);
    }

    //assert(brushCurve.coords.size() > 1);
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

void PaintScene::execute(const QString& data) {
    std::vector<unsigned char> receivedCommand = base64_decode(data.toStdString());

    unsigned char currentCommand = receivedCommand[0];
    if (currentCommand == 'B') {
        executeBrush(receivedCommand);
    } else if (currentCommand == 'L') {
        //executeLine(data);
    } else if (currentCommand == 'C') {
        //executeCircle(data);
    } else if (currentCommand == 'R') {
        //executeRectangle(data);
    } else if (currentCommand == 'T') {
        //executeText(data);
    } else {
        assert(false);;
    }
}
void PaintScene::executeBrush(const std::vector<unsigned char> &data) {
    union {
        unsigned char bytes[2];
        int16_t size = 0;
    } brushSizeExec;

    brushSizeExec.bytes[0] = data[1];
    brushSizeExec.bytes[1] = data[2];

    QColor brushColorExec((unsigned char)data[3], (unsigned char)data[4],
                          (unsigned char)data[5], (unsigned char)data[6]);

    union {
        unsigned char bytes[4];
        int16_t coord[2] = {0, 0}; // 0 - x, 1 - y
    } pointUnion;

    pointUnion.bytes[0] = data[7];
    pointUnion.bytes[1] = data[8];
    pointUnion.bytes[2] = data[9];
    pointUnion.bytes[3] = data[10];

    size_t numb_of_points = (data.size() - 7) / 4;// 7 bytes - command + color + size
                                                  // 4 bytes - each point
    if (numb_of_points > 1) {
        int16_t previousPoint_x = pointUnion.coord[0];
        int16_t previousPoint_y = pointUnion.coord[1];
        for (size_t i = 1; i < numb_of_points; ++i) {
            pointUnion.bytes[0] = data[7 + i * 4];
            pointUnion.bytes[1] = data[8 + i * 4];
            pointUnion.bytes[2] = data[9 + i * 4];
            pointUnion.bytes[3] = data[10 + i * 4];
            int16_t currentPoint_x = pointUnion.coord[0];
            int16_t currentPoint_y = pointUnion.coord[1];

            addLine(currentPoint_x, currentPoint_y,
                    previousPoint_x, previousPoint_y,
                    QPen(brushColorExec, brushSizeExec.size, Qt::SolidLine, Qt::RoundCap));

            previousPoint_x = currentPoint_x;
            previousPoint_y = currentPoint_y;
        }
    } else {
        // draw circle
    }
}

void PaintScene::setStatus(const char tool){
    if (currentTool == tool) {
        currentTool = 'E';
    } else {
        currentTool = tool;
    }
}
