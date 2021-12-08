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

        void execute() {
            scene->addLine(prev.x(), prev.y(),
                           next.x(), next.y(),
                           QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap));
        }
    };
}// namespace command
