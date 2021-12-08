//
// Created by eremey on 16.11.2021.
//
#pragma once
#include <QLabel>
#include <QPixmap>
#include <QColor>

class Palette : public QLabel
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = 0);
    ~Palette() = default;

    void SetColor(int red, int green, int blue, int opacity = 255);

    //void SetGreen(int value);
private:
    QColor color;
    //QPixmap *pixmap;

public slots:
    void SetRed(int value);
    void SetGreen(int value);
    void SetBlue(int value);

private slots:
    //void slotTimer();
    //void slotBrush();
};