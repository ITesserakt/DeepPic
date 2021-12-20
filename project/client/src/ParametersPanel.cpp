//
// Created by eremey on 18.11.2021.
//

#include "ParametersPanel.h"

ParametersPanel::ParametersPanel(QWidget *parent) : QToolBar(parent) {
    setFixedHeight(40);
};

void ParametersPanel::setBrush(qreal brushSize, const QColor& brushColor) {
    QWidget *dummy = new QWidget(this);
    dummy->setFixedWidth(40);
    addWidget(dummy);

    Changer *size_changer = new Changer("Size", brushSize, 500, this);
    addWidget(size_changer);
    connect(size_changer, QOverload<int>::of(&Changer::valueChanged), this, &ParametersPanel::BrushSizeChangedSlot);

    addSeparator();


    Palette *palette = new Palette(this);
    palette->SetColor(brushColor.red(), brushColor.green(), brushColor.blue());

    Changer *red_changer = new Changer("Red", brushColor.red(), 255, this);
    connect(red_changer, QOverload<int>::of(&Changer::valueChanged), this, &ParametersPanel::BrushRedChangedSlot);
    connect(red_changer, QOverload<int>::of(&Changer::valueChanged), palette, (&Palette::SetRed));
    addWidget(red_changer);

    Changer *green_changer = new Changer("Green", brushColor.green(), 255, this);
    connect(green_changer, QOverload<int>::of(&Changer::valueChanged), this, &ParametersPanel::BrushGreenChangedSlot);
    connect(green_changer, QOverload<int>::of(&Changer::valueChanged), palette, (&Palette::SetGreen));

    addWidget(green_changer);

    Changer *blue_changer = new Changer("Blue", brushColor.blue(), 255, this);
    connect(blue_changer, QOverload<int>::of(&Changer::valueChanged), this, &ParametersPanel::BrushBlueChangedSlot);
    connect(blue_changer, QOverload<int>::of(&Changer::valueChanged), palette, (&Palette::SetBlue));

    addWidget(blue_changer);


    //        Palette palette;
    addWidget(palette);
    addSeparator();

    Changer *opacity_changer = new Changer("Opacity", 100, 100, this);
    connect(opacity_changer, QOverload<int>::of(&Changer::valueChanged), this, &ParametersPanel::BrushOpacityChangedSlot);
    addWidget(opacity_changer);

    setStyleSheet(QString("QToolBar {spacing: %1}").arg(10));
}

void ParametersPanel::BrushSizeChangedSlot(int newSize) {
    emit(BrushSizeChanged(newSize));
}
void ParametersPanel::BrushRedChangedSlot(int newValue) {
    emit(BrushRedChanged(newValue));
}
void ParametersPanel::BrushGreenChangedSlot(int newValue) {
    emit(BrushGreenChanged(newValue));
}
void ParametersPanel::BrushBlueChangedSlot(int newValue) {
    emit(BrushBlueChanged(newValue));
}
void ParametersPanel::BrushOpacityChangedSlot(int newValue) {
    emit(BrushOpacityChanged((newValue * 255) / 100));
}
