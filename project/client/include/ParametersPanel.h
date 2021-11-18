//
// Created by eremey on 18.11.2021.
//
#pragma once
#include <QToolBar>
#include "Changer.h"
#include "Palette.h"
#include <QWidget>

class ParametersPanel : public QToolBar {
    Q_OBJECT

public:
    explicit ParametersPanel(QWidget *parent = 0);
    ~ParametersPanel() = default;

public slots:
    void setBrush();

private:
    Changer* changers;
};

