//
// Created by eremey on 16.11.2021.
//
#include "Palette.h"
#include <iostream>

Palette::Palette(QWidget *parent) :
    QLabel(parent), color(0,0,0,255) {
    SetRed(255);
}
void Palette::SetGreen(int value) {
    color.setGreen(value);
    QPixmap pixmap(15, 15);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::SetRed(int value) {
    color.setRed(value);
    QPixmap pixmap(15, 15);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::SetBlue(int value) {
    color.setBlue(value);
    QPixmap pixmap(15, 15);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
void Palette::SetColor(int red, int green, int blue, int opacity) {
    color.setRgb(red, green, blue, opacity);
    QPixmap pixmap(15, 15);
    pixmap.fill( color );

    this->setPixmap(pixmap);
}
