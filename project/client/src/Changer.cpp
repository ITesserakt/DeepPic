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

    spin_box = new QSpinBox;
    spin_box->setValue(value);
    spin_box->setMaximum(max_value);
    spin_box->setFixedWidth(50);
    this->addWidget(spin_box);

    slider = new QSlider(Qt::Horizontal);
    slider->setFixedWidth(100);
    slider->setValue(value);
    slider->setMaximum(max_value);
    this->addWidget(slider);

    this->setFixedWidth(size_label->width() + spin_box->width() + slider->width() + 20);
    std::cout << size_label->width() << " " << spin_box->width() << " " << slider->width();

    connect(slider, QOverload<int>::of(&QSlider::valueChanged), this,
            (&Changer::valueChangedSlot));
    connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged), this,
            (&Changer::valueChangedSlot));

    this->setStyleSheet(QString("QToolBar {spacing: %1}").arg(5));


}
void Changer::valueChangedSlot(int value) {
    _value = value;
    spin_box->value() == value ? slider->setValue(value) : spin_box->setValue(value);
    emit valueChanged(value);
}
