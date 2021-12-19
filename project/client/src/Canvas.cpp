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


    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    render(&painter);
    image.save(filePath, "JPG");
}
void Canvas::saveAsImageSlot() {
    filePath = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), ".");
    QImage image(scene()->width(), scene()->height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    setRenderHint(QPainter::Antialiasing);
    render(&painter);
    image.save(filePath, "JPG");
}
void Canvas::closeImageSlot() {

}

QVector<unsigned char> Canvas::GetImageVector() {
    QVector<unsigned char> ImageVector;

    union{
        unsigned char bytes[4];
        uint16_t height;
        uint16_t width;
    }byte_size;

    byte_size.height = height();
    byte_size.width = width();

    for (int i = 0; i < 4; ++i) {
        ImageVector.push_back(byte_size.bytes[i]);
    }


    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    render(&painter);




    for (int i = 0; i < width(); ++i) {
        for (int j = 0; j < height(); ++j) {
            QColor color = image.pixelColor(i, j);

            ImageVector.push_back(color.red());
            ImageVector.push_back(color.green());
            ImageVector.push_back(color.blue());
        }
    }

    return ImageVector;
}
void Canvas::setImageSlot(QVector<unsigned char> image) {
    //
}
