//
// Created by tesserakt on 08.12.2021.
//

#include "SocketClient.h"

unsigned int SocketClient::ID = 0;

unsigned int SocketClient::id() {
    return id_;
}

cppcoro::task<> SocketClient::send(std::ostringstream &data) {
    auto str = data.str();
    co_await socket->send(str.data(), str.size());
}

cppcoro::task<> SocketClient::onReceived(std::string data) {
    co_return;
}

SocketClient::SocketClient(shared_socket socket) : socket(socket) {}