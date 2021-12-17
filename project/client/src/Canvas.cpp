//
// Created by eremey on 01.12.2021.
//

#include "Canvas.h"
#include <QFileDialog>
#include <iostream>////

Canvas::Canvas(QWidget *parent) :
    QGraphicsView(parent) {
}

void Canvas::openImageSlot() {
    QString path = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    auto *image = new QImage;
    image->load(path);

    if(image->height() < height()) {
        setFixedHeight(image->height() + 16);  // 16 - slider height
    }
    if(image->width() < width()) {
        setFixedWidth(image->width() + 16);
    }

    setSceneRect(0, 0, image->width(), image->height());
    scene()->addPixmap(QPixmap::fromImage(*image));
    delete image;
}

void Canvas::saveImageSlot() {
    if (filePath == nullptr) {
        filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"), ".");
    }

    QImage image(scene()->width(), scene()->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    scene()->render(&painter);
    image.save(filePath, "JPG");
}
void Canvas::saveAsImageSlot() {
    filePath = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), ".");

    QImage image(scene()->width(), scene()->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    scene()->render(&painter);
    image.save(filePath, "JPG");
}
void Canvas::closeImageSlot() {

}
