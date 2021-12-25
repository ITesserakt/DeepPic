//
// Created by eremey on 08.12.2021.
//
#pragma once
#include <QToolBar>
#include <QPushButton>
#include <QLabel>

class Connector : public QToolBar {
    Q_OBJECT

public:
    explicit Connector(QWidget *parent = nullptr);
    ~Connector();

signals:
    void writeSignal(std::string& message);

public slots:
    void failedShareSlot();
    void successfulShareSlot(const QString& message);

    void failedConnectSlot();
    void successfulConnectSlot(const QString& message);

private:
    QPushButton* connectButton;
    QPushButton* shareButton;
    QPushButton* canselButton;
    //QLabel* message;

    QString address;
    QString token;
    QString port;

signals:
    void initSignal();
//    void shareSignal();
//    void connectSignal();
private slots:
    void initSlot();
    void connectSlot();

    void writeConnectionSlot();
    void writeShareSlot();


    void addressChangedSlot(const QString &text);
    void portChangedSlot(const QString &text);
    void tokenChangedSlot(const QString &text);

};
