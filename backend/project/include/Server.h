//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <cppcoro/async_scope.hpp>
#include <cppcoro/cancellation_source.hpp>
#include <cppcoro/io_service.hpp>
#include <cppcoro/net/ip_address.hpp>
#include <cppcoro/net/socket.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>
#include <iostream>
#include <memory>

#include "Serializer.h"
#include "SocketClient.h"
#include "Transporter.h"

using IStream = std::istringstream;
using OStream = std::ostringstream;

template <Serializer<IStream, OStream> S>
class Server : public Transporter<IStream, S> {
private:
    cppcoro::task<> handleIncoming(cppcoro::net::socket &client) {
        using namespace std::chrono_literals;
        using namespace std::string_literals;

        char buffer[1024];

        try {
            while (isActive) {
                std::size_t read;
                do {
                    read = co_await client.recv(buffer, 1024, source.token());
                } while (read != 0 && buffer[read] != '\u0004');
                if (read != 0)
                    co_await onReceived(std::string{buffer, buffer + read});
            }
        } catch (const cppcoro::operation_cancelled &) {
        } catch (const std::exception &ex) { throw std::runtime_error(std::string("Unknown exception happened: ") + ex.what()); }
    }

protected:
    cppcoro::io_service async;
    cppcoro::cancellation_source source;
    unsigned int serverId = -1;
    bool isActive = true;

    using shared_socket = std::shared_ptr<cppcoro::net::socket>;
    using shared_client = std::shared_ptr<SocketClient>;

    virtual void clientAttached(shared_socket client) = 0;

    cppcoro::task<> loop(cppcoro::net::socket &&socket) {
        cppcoro::async_scope scope;
        std::exception_ptr currentEx;
        try {
            while (isActive) {
                auto connection = std::make_shared<cppcoro::net::socket>(cppcoro::net::socket::create_tcpv4(async));
                co_await socket.accept(*connection, source.token());
                clientAttached(connection);
                scope.spawn(handleIncoming(*connection));
            }
        } catch (const cppcoro::operation_cancelled &) {
        } catch (...) { currentEx = std::current_exception(); }

        co_await scope.join();
        if (currentEx) std::rethrow_exception(currentEx);
    }

    virtual cppcoro::task<> onReceived(std::string data) = 0;

public:
    unsigned int id() {
        return serverId;
    }

    cppcoro::task<> start(const cppcoro::net::ipv4_endpoint &endpoint) {
        auto exit = cppcoro::on_scope_exit([&] { async.stop(); });
        try {
            auto serverSocket = cppcoro::net::socket::create_tcpv4(async);
            serverId = serverSocket.native_handle();
            serverSocket.bind(endpoint);
            serverSocket.listen();
            co_await loop(std::move(serverSocket));
        } catch (const cppcoro::operation_cancelled &) {
        } catch (const std::exception &ex) {
            throw std::runtime_error(std::string("Unknown exception happened: ") + ex.what());
        }
    }

    cppcoro::task<> listen() {
        async.process_events();
        co_return;
    }

    virtual cppcoro::task<> shutdown() = 0;
};
