//
// Created by eremey on 16.11.2021.
//
#pragma once
#include <QToolBar>

#include <QSpinBox>
#include <QSlider>

class Changer : public QToolBar
{
    Q_OBJECT

public:
    explicit Changer(QString name = "", int value = 30, int max_value = 255, QWidget *parent = 0);
    ~Changer() = default;

private:

    QSpinBox *changerSpinBox;
    QSlider *changerSlider;

public:

signals:
    void valueChanged(int value);

private slots:
    void valueChangedSlot(int value);
};