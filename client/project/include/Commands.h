//
// Created by tesserakt on 07.12.2021.
//

#pragma once

#include <QGraphicsScene>
#include <iostream>

#include "command/Command.h"

namespace command {
    struct UI : Command {
        QGraphicsScene *scene = nullptr;// it will set up later
    };

    struct BrushStarted : UI {
        QPointF next;
        qreal brushSize;
        QColor brushColor;

        BrushStarted(QPointF next, qreal size, QColor color) : next(next), brushSize(size), brushColor(color) {}

        BrushStarted(double x, double y, double size, int r, int g, int b)
            : next(x, y), brushSize(size), brushColor(r, g, b) {}

        double getX() const { return next.x(); }
        double getY() const { return next.y(); }
        double getSize() const { return brushSize; }
        int getRed() const { return brushColor.red(); }
        int getGreen() const { return brushColor.green(); }
        int getBlue() const { return brushColor.blue(); }

        void execute() {
            scene->addEllipse(next.x() - brushSize / 2,
                              next.y() - brushSize / 2,
                              brushSize,
                              brushSize,
                              QPen(Qt::NoPen),
                              QBrush(brushColor));
        }
    };

    struct BrushMoved : UI {
        QPointF prev, next;
        qreal brushSize;
        QColor brushColor;

        BrushMoved(QPointF prev, QPointF next, qreal size, QColor color)
            : prev(prev), next(next), brushSize(size), brushColor(color) {}

        BrushMoved(double x, double y, double nextX, double nextY, double size, int r, int g, int b)
            : prev(x, y), next(nextX, nextY), brushSize(size), brushColor(r, g, b) {}

        double getX() const { return prev.x(); }
        double getY() const { return prev.y(); }
        double getNextX() const { return next.x(); }
        double getNextY() const { return next.y(); }
        double getSize() const { return brushSize; }
        int getRed() const { return brushColor.red(); }
        int getGreen() const { return brushColor.green(); }
        int getBlue() const { return brushColor.blue(); }

        void execute() {
            scene->addLine(prev.x(), prev.y(),
                           next.x(), next.y(),
                           QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap));
        }
    };
}// namespace command

JSONCONS_ALL_CTOR_GETTER_TRAITS(command::BrushStarted, getX, getY, getSize, getRed, getGreen, getBlue)
JSONCONS_ALL_CTOR_GETTER_TRAITS(command::BrushMoved, getX, getY, getNextX, getNextY, getSize, getRed, getGreen, getBlue)