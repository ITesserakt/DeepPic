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
    explicit Palette(QWidget *parent = 0, size_t width = 73, size_t height = 15);
    ~Palette() = default;

    void SetColor(int red, int green, int blue, int opacity = 255);
    const QColor& getColor() const;

    //void setGreen(int value);
private:
    QColor color;
    //QPixmap *pixmap;

public slots:
    void setRed(int value);
    void setGreen(int value);
    void setBlue(int value);
    void setOpacity(int value);

private slots:
    //void slotTimer();
    //void slotBrush();
};