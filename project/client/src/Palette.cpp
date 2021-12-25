//
// Created by eremey on 16.11.2021.
//
#include "Palette.h"
#include <iostream>

Palette::Palette(QWidget *parent, size_t width, size_t height) :
    QLabel(parent), color(0,200,0,255) {
    setRed(255);
}
void Palette::setGreen(int value) {
    color.setGreen(value);
    QPixmap pixmap(30, 30);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::setRed(int value) {
    color.setRed(value);
    QPixmap pixmap(30, 30);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::setBlue(int value) {
    color.setBlue(value);
    QPixmap pixmap(30, 30);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::SetColor(int red, int green, int blue, int opacity) {
    color.setRgb(red, green, blue, opacity);
    QPixmap pixmap(30, 30);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
const QColor& Palette::getColor() const{
    return color;
}
void Palette::setOpacity(int value) {
    color.setAlpha(value);
}
