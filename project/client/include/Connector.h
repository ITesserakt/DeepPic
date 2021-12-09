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
    void successfulShareSlot(std::string& message);

    void failedConnectSlot();
    void successfulConnectSlot(std::string& message);

private:
    QPushButton* connectButton;
    QPushButton* shareButton;
    QPushButton* canselButton;
    //QLabel* message;

    std::string address = "";
    std::string token = "";

signals:
    void initSignal();
//    void shareSignal();
//    void connectSignal();
private slots:
    void initSlot();
    void shareSlot();
    void connectSlot();

    void writeConnectionSlot();
    void writeShareSlot();

    void successfulConnectSlot();


    void addressChangedSlot(const QString &text);
    void tokenChangedSlot(const QString &text);

};
