//
// Created by eremey on 08.12.2021.
//
#include <nlohmann/json.hpp>

#include "Connector.h"

#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

#include "iostream"

using json = nlohmann::json;

Connector::Connector(QWidget *parent) : QToolBar(parent) {
    connect(this, &Connector::initSignal, this, &Connector::initSlot);
//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);
    emit(initSignal());
}

void Connector::initSlot() {
    clear();
    shareButton = new QPushButton("Share document", this);
    //connect(shareButton, &QPushButton::clicked, this, &Connector::shareSlot);
    connect(shareButton, &QPushButton::clicked, this, &Connector::writeShareSlot);
    connectButton = new QPushButton("Connect to document", this);
    connect(connectButton, &QPushButton::clicked, this, &Connector::connectSlot);

//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);

    addWidget(shareButton);
    addWidget(connectButton);
}

void Connector::connectSlot() {
    clear();
    auto addressLbl = new QLabel("Address:", this);
    addWidget(addressLbl);
    auto addressEdt = new QLineEdit(this);
    connect(addressEdt, &QLineEdit::textEdited, this, &Connector::addressChangedSlot);
    addWidget(addressEdt);
    auto tokenLbl = new QLabel("Token:", this);
    addWidget(tokenLbl);
    auto tokenEdt = new QLineEdit(this);
    connect(tokenEdt, &QLineEdit::textEdited, this, &Connector::tokenChangedSlot);
    addWidget(tokenEdt);

    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    connectButton = new QPushButton("Connect", this);
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
    address = text;
}

void Connector::tokenChangedSlot(const QString &text) {
    token = text;
}

void Connector::writeConnectionSlot() {
    // TODO: 2) change connection message
    json message_json = {{"target",     "auth"},
                         {"auth_token", token.toStdString()},
                         {"address",    address.toStdString()}};
    std::string message = message_json.dump();
    emit(writeSignal(message));
}

void Connector::writeShareSlot() {
    json message_json = {{"target",     "sharing_document"},
                         {"auth_token", token.toStdString()},
                         {"address",    address.toStdString()}};
    std::string message = message_json.dump();
    emit(writeSignal(message));
}

void Connector::failedShareSlot() {
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message = new QLabel(this);

    clear();
    message->setText("failed");
    addWidget(message);
    addWidget(canselButton);
}

void Connector::failedConnectSlot() {
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message = new QLabel(this);

    clear();
    message->setText("failed");
    addWidget(message);
    addWidget(canselButton);
}

void separate(const QString& message, QString& l, QString& r, char separator) {

    l = QString::fromStdString(message.toStdString().substr(0, message.toStdString().find(separator))); // rewrite
    r = QString::fromStdString(message.toStdString().substr(message.toStdString().find(separator) + 1, message.size() - 1));
}

void Connector::successfulShareSlot(const QString& message) {
//    std::string separator = "|";
//    address = message.substr(0, message.find(separator));
//    token = message.substr(message.find(separator) + 1, message.size() - 1);
    separate(message, address, token, '|');

    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label = new QLabel(this);

    clear();
    QString msg = "You're sharing the document.\nAddress: " + address + "\nToken: " + token;
    label->setText(msg);
    addWidget(label);
    addWidget(canselButton);
}

void Connector::successfulConnectSlot(const QString &message) {
    separate(message, address, token, '|');

    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label = new QLabel(this);

    clear();
    QString msg = "You've joined the document.\nAddress: " + address + "\nToken: " + token;
    label->setText(msg);
    addWidget(label);
    addWidget(canselButton);
}