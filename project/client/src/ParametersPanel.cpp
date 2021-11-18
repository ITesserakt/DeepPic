//
// Created by eremey on 18.11.2021.
//

#include "ParametersPanel.h"

ParametersPanel::ParametersPanel(QWidget *parent) : QToolBar(parent) {
    setFixedHeight(40);
};

void ParametersPanel::setBrush() {
    QWidget *dummy = new QWidget();
    dummy->setFixedWidth(40);
    addWidget(dummy);

    Changer *size_changer = new Changer("Size", 30, 500, this);
    addWidget(size_changer);
    addSeparator();


    Palette *palette = new Palette;
    palette->SetColor(0, 180, 40, 255);

    Changer *red_changer = new Changer("Red", 0, 255, this);
    addWidget(red_changer);

    Changer *green_changer = new Changer("Green", 180, 255, this);
    addWidget(green_changer);

    Changer *blue_changer = new Changer("Blue", 40, 255, this);
    addWidget(blue_changer);


    //        Palette palette;
    addWidget(palette);
    addSeparator();

    Changer *opacity_changer = new Changer("Opacity", 255, 255, this);
    addWidget(opacity_changer);


    setStyleSheet(QString("QToolBar {spacing: %1}").arg(10));
}
