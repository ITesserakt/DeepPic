//
// Created by tesserakt on 09.12.2021.
//

#pragma once

#include <cppcoro/async_scope.hpp>
#include <cppcoro/cancellation_source.hpp>
#include <cppcoro/io_service.hpp>
#include <cppcoro/net/ip_address.hpp>
#include <cppcoro/net/socket.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>

#include "JsonSerializer.h"
#include "Transporter.h"

template <Serializer<std::istringstream, std::ostringstream> S>
class Client : public Transporter<std::istringstream, S> {
public:
    using shared_socket = std::shared_ptr<cppcoro::net::socket>;

protected:
    cppcoro::io_service async;
    cppcoro::cancellation_source source;
    unsigned int clientId = -1;
    shared_socket socket;

public:
    cppcoro::task<> connectTo(const cppcoro::net::ip_endpoint &endpoint) {
        socket = std::make_shared<cppcoro::net::socket>(cppcoro::net::socket::create_tcpv4(async));
        clientId = socket->native_handle();

        co_await socket->connect(endpoint, source.token());
    }

    unsigned int id() override { return clientId; }
};