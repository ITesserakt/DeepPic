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

    QVector<unsigned char> GetImageVector();

public slots:
    void openImageSlot();
    void saveImageSlot();
    void saveAsImageSlot();
    void saveImageToSlot(const QString& path);
    void closeImageSlot();
    void setImageSlot( const QVector<unsigned char> imageVector);
    

private:
    QString filePath = nullptr;
    void save(const QString& path);

    size_t default_width;
};