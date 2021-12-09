//
// Created by eremey on 08.12.2021.
//

#include "Connector.h"

#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QString>


Connector::Connector(QWidget *parent) : QToolBar(parent) {
    connect(this, &Connector::initSignal, this, &Connector::initSlot);
//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);
    emit(initSignal());
}

void Connector::initSlot() {
    clear();
    shareButton =  new QPushButton("Share document", this);
    //connect(shareButton, &QPushButton::clicked, this, &Connector::shareSlot);
    connect(shareButton, &QPushButton::clicked, this, &Connector::writeShareSlot);
    connectButton = new QPushButton("Connect to document", this);
    connect(connectButton, &QPushButton::clicked, this, &Connector::connectSlot);

//    connect(this, &Connector::connectSignal, this, &Connector::connectSlot);
//    connect(this, &Connector::shareSignal, this, &Connector::shareSlot);

    addWidget(shareButton);
    addWidget(connectButton);
}
void Connector::shareSlot() {
    // TODO share()
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message =  new QLabel(this);

    clear();
    QString msg = "You're sharing the document.\nAddress: \nToken: ";
    message->setText(msg);
    addWidget(message);
    addWidget(canselButton);
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
void Connector::successfulConnectSlot() {
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message =  new QLabel(this);

    clear();
    QString msg = QString::fromStdString("You've joined the document.\nAddress: " + address + "\nToken: " + token);
    message->setText(msg);
    addWidget(message);
    addWidget(canselButton);
}
void Connector::addressChangedSlot(const QString &text) {
    token = text.toStdString();
}
void Connector::tokenChangedSlot(const QString &text) {
    address = text.toStdString();
}
void Connector::writeConnectionSlot() {
    // TODO: change separator
    char separator = '$';
    std::string message = address + separator + token;
    emit(writeSignal(message));
}
void Connector::writeShareSlot() {
    // TODO: change share message
    std::string message = "$";
    emit(writeSignal(message));
}
void Connector::failedShareSlot() {
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message =  new QLabel(this);

    clear();
    message->setText("failed");
    addWidget(message);
    addWidget(canselButton);
}
void Connector::failedConnectSlot() {
    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto message =  new QLabel(this);

    clear();
    message->setText("failed");
    addWidget(message);
    addWidget(canselButton);
}
void Connector::successfulShareSlot(std::string &message) {
    // TODO: add message parser
    address = "";
    token = "";
    //

    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label =  new QLabel(this);

    clear();
    QString msg = QString::fromStdString("You're sharing the document.\nAddress: \nToken: " + token);
    label->setText(msg);
    addWidget(label);
    addWidget(canselButton);
}
void Connector::successfulConnectSlot(std::string& message) {
    // TODO: add message parser
    address = "";
    token = "";
    //

    canselButton = new QPushButton("Cansel", this);
    connect(canselButton, &QPushButton::clicked, this, &Connector::initSlot);
    auto label =  new QLabel(this);

    clear();
    QString msg = QString::fromStdString("You've joined the document.\nAddress: " + address + "\nToken: " + token);
    label->setText(msg);
    addWidget(label);
    addWidget(canselButton);
}
