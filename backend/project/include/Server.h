//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <SmallVec.hpp>
#include <cppcoro/async_scope.hpp>
#include <cppcoro/cancellation_source.hpp>
#include <cppcoro/net/ip_address.hpp>
#include <cppcoro/net/socket.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>
#include <iostream>
#include <memory>

#include "BoostSerializer.h"
#include "Serializer.h"

using IStream = std::istringstream;
using OStream = std::ostringstream;

template <Serializer<IStream, OStream> S>
class Server {
private:
    std::unique_ptr<S> serializer;

    cppcoro::task<> handleIncoming(cppcoro::net::socket &client) {
        using namespace std::chrono_literals;
        using namespace std::string_literals;

        char buffer[1024];

        while (isActive) {
            std::size_t read = co_await client.recv(buffer, 1024, source.token());
            std::cout << "Read " << read << " next bytes..." << std::endl;
            onReceived(std::string{buffer, buffer + read});
        }
    }

protected:
    cppcoro::io_service async;
    cppcoro::cancellation_source source;
    unsigned int serverId = -1;
    bool isActive = true;

    using shared_socket = std::shared_ptr<cppcoro::net::socket>;

    virtual cppcoro::task<> broadcast(std::string) = 0;

    virtual void clientAttached(shared_socket client) = 0;

    cppcoro::task<> loop(cppcoro::net::socket &&socket) {
        cppcoro::async_scope scope;
        while (isActive) {
            auto connection = std::make_shared<cppcoro::net::socket>(cppcoro::net::socket::create_tcpv4(async));
            co_await socket.accept(*connection, source.token());
            clientAttached(connection);
            scope.spawn(handleIncoming(*connection));
        }
        co_await scope.join();
    }

    virtual void onReceived(std::string data) = 0;

public:
    unsigned int id() {
        return serverId;
    }

    template <typename T>
    cppcoro::task<> broadcast(T value) {
        std::ostringstream os;
        co_await broadcast(serializer->serialize(value, os).str());
    }

    template <typename... Args>
    void createSerializer(Args &&...args) {
        serializer = std::make_unique<S>(std::forward<Args>(args)...);
    }

    cppcoro::task<> start(const cppcoro::net::ipv4_endpoint &endpoint) {
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
        co_await cppcoro::when_all(
                [&]() -> cppcoro::task<> { async.process_events(); co_return ; }(),
                [&]() -> cppcoro::task<> { cppcoro::on_scope_exit([&] { async.stop(); }); co_return ; }());
    }

    virtual cppcoro::task<> shutdown() = 0;
};
