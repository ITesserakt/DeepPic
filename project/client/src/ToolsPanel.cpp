//
// Created by eremey on 18.11.2021.
//

#include <QAction>
#include <QPixmap>
#include <ToolsPanel.h>


#include "ToolsPanel.h"

ToolsPanel::ToolsPanel(QWidget *parent) : QToolBar(parent) {
    QPixmap brush_pm("brush.png");

    QAction *brush = addAction(QIcon(brush_pm), "B");

    QPixmap pixmap(brush_pm.size());
    pixmap.fill( Qt::white);

    QAction *lasso = addAction(pixmap, "L");
    QAction *elaser = addAction(pixmap, "E");
    QAction *filler = addAction(pixmap, "F");
    QAction *scaling = addAction(pixmap, "S");
    QAction *loupe = addAction(pixmap, "L");
    QAction *primitives = addAction(pixmap, "P");
}
