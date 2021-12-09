//
// Created by tesserakt on 08.12.2021.
//

#pragma once

#include <cppcoro/net/socket.hpp>

#include "JsonSerializer.h"
#include "Transporter.h"

class SocketClient : public Transporter<std::istringstream, JsonSerializer> {
public:
    using shared_socket = std::shared_ptr<cppcoro::net::socket>;

private:
    static unsigned int ID;
    unsigned int id_ = ID++;
    shared_socket socket;

public:
    unsigned int id() override;

    cppcoro::task<> send(std::ostringstream &data) override;

    cppcoro::task<> onReceived(std::string data) override;

    SocketClient(shared_socket socket);
};