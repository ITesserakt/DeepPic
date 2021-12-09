//
// Created by eremey on 08.12.2021.
//

#include "Connector.h"

#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

#include "iostream"

Connector::Connector(QWidget *parent) : QToolBar(parent) {
    connect(this, &Connector::initSignal, this, &Connector::initSlot);
//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);
    emit(initSignal());
}

void Connector::initSlot() {
    clear();
    auto shareButton =  new QPushButton("Share document", this);
    //connect(shareButton, &QPushButton::clicked, this, &Connector::shareSlot);
    connect(shareButton, &QPushButton::clicked, this, &Connector::writeShareSlot);
    auto connectButton = new QPushButton("Connect to document", this);
    connect(connectButton, &QPushButton::clicked, this, &Connector::connectSlot);

//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);

    addWidget(shareButton);
    addWidget(connectButton);
}

void Connector::connectSlot() {
    clear();
    auto addressLbl =  new QLabel("Address:", this);
    addWidget(addressLbl);
    auto addressEdt =  new QLineEdit(this);
    connect(addressEdt, &QLineEdit::textEdited, this, &Connector::addressChangedSlot);
    addWidget(addressEdt);
    auto tokenLbl = new QLabel("Token:",this);
    addWidget(tokenLbl);
    auto tokenEdt = new QLineEdit(this);
    connect(tokenEdt, &QLineEdit::textEdited, this, &Connector::tokenChangedSlot);
    addWidget(tokenEdt);

    auto canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto connectButton = new QPushButton("Connect", this);
    connect(connectButton, &QPushButton::clicked, this, &Connector::writeConnectionSlot);


    addSeparator();
    addWidget(connectButton);
    addWidget(canselButton);
    addSeparator();
}
Connector::~Connector() {
    clear();
}

void Connector::addressChangedSlot(const QString &text) {
    token = text.toStdString();
}
void Connector::tokenChangedSlot(const QString &text) {
    address = text.toStdString();
}
void Connector::writeConnectionSlot() {
    // TODO: 2) change connection message
    char separator = '|';
    std::string message = address + separator + token;
    emit(writeSignal(message));
}
void Connector::writeShareSlot() {
    // TODO: 3) change share message
    std::string message = "$";
    emit(writeSignal(message));
}
void Connector::failedShareSlot() {
    QString message = "Share failed";
    fail(message);
}
void Connector::failedConnectSlot() {
    QString message = "Connection failed";
    fail(message);
}

void separate(const std::string &message, std::string& l, std::string& r, char separator) {
    l = message.substr(0, message.find(separator));
    r = message.substr(message.find(separator) + 1, message.size() - 1);
}

void Connector::successfulShareSlot(std::string &message) {
    separate(message, address, token, '|');

    QString msg = QString::fromStdString("You're sharing the document.\nAddress: "+ address + "\nToken: " + token);

    success(msg);
}
void Connector::successfulConnectSlot(std::string& message) {
    separate(message, address, token, '|');

    QString msg = QString::fromStdString("You've joined the document.\nAddress: " + address + "\nToken: " + token);

    success(msg);
}
void Connector::success(QString &message) {
    auto canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label =  new QLabel(this);
    label->setText(message);

    clear();
    addWidget(label);
    addWidget(canselButton);
}
void Connector::fail(QString &message) {
    auto okButton = new QPushButton("Ok", this);
    connect(okButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label =  new QLabel(this);

    clear();
    label->setText(message);
    addWidget(label);
    addWidget(okButton);
}
