//
// Created by eremey on 16.11.2021.
//
#include "Changer.h"

#include <QLabel>
#include <QSlider>

#include <iostream>

Changer::Changer(QString name, int value, int max_value, QWidget *parent) :
    QToolBar(parent) {

    QLabel *size_label = new QLabel(name, this);
    this->addWidget(size_label);
    size_label->setFixedWidth(name.length() * 9);

    changerSpinBox = new QSpinBox(this);
    changerSpinBox->setValue(value);
    changerSpinBox->setMaximum(max_value);
    changerSpinBox->setFixedWidth(50);
    this->addWidget(changerSpinBox);

    changerSlider = new QSlider(Qt::Horizontal, this);
    changerSlider->setFixedWidth(100);
    changerSlider->setValue(value);
    changerSlider->setMaximum(max_value);
    this->addWidget(changerSlider);

    this->setFixedWidth(size_label->width() + changerSpinBox->width() + changerSlider->width() + 20);
    std::cout << size_label->width() << " " << changerSpinBox->width() << " " << changerSlider->width();

    connect(changerSlider, QOverload<int>::of(&QSlider::valueChanged), this,
            (&Changer::valueChangedSlot));
    connect(changerSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            (&Changer::valueChangedSlot));

    this->setStyleSheet(QString("QToolBar {spacing: %1}").arg(5));


}
void Changer::valueChangedSlot(int value) {
    changerSpinBox->value() == value ? changerSlider->setValue(value) : changerSpinBox->setValue(value);
    emit valueChanged(value);
}
