//
// Created by eremey on 01.12.2021.
//
#pragma once
#include <QLabel>
#include <QPixmap>
#include <QColor>
#include<QGraphicsView>

class Canvas : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = 0);
    ~Canvas() = default;
};