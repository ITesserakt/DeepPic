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
    std::string address;
    std::string token;

signals:
    void initSignal();
private slots:
    void initSlot();
    void connectSlot();

    void writeConnectionSlot();
    void writeShareSlot();


    void addressChangedSlot(const QString &text);
    void tokenChangedSlot(const QString &text);

private:
    void success(QString &message);
    void fail(QString &message);
};
