//
// Created by tesserakt on 09.12.2021.
//

#pragma once

#include <iostream>

#include "JsonSerializer.h"
#include "NetworkCommandExecutor.h"
#include "Server.h"
#include "command/NetworkCommand.h"

class CommonServer : public Server<JsonSerializer> {
private:
    CommandBus bus_;
    std::vector<shared_socket> clients;
    std::vector<std::function<cppcoro::task<>(NetworkCommand &)>> callbacks;

public:
    cppcoro::task<> shutdown() override {
        for (auto &client : clients) {
            co_await client->disconnect();
        }
        isActive = false;
        source.request_cancellation();
    }

    template <typename F>
    void registerCallback(F &&callback) {
        callbacks.emplace_back(std::forward<F>(callback));
    }

protected:
    cppcoro::task<> send(std::ostringstream &data) override {
        auto str = data.str();
        std::cout << "Broadcasting: " << str << std::endl;
        cppcoro::async_scope scope;
        for (auto &client : clients)
            scope.spawn([&]() -> cppcoro::task<> {
                co_await client->send(str.data(), str.size(), source.token());
            }());
        co_await scope.join();
    }

    void clientAttached(shared_socket client) override {
        std::cout << "New client attached: " << client->native_handle()
                  << ", from: " << client->remote_endpoint().to_string() << std::endl;
        clients.emplace_back(client);
        auto clientWrapper = std::make_shared<SocketClient>(client);
        bus_.addExecutor<NetworkCommandExecutor>(clientWrapper);
    }

    cppcoro::task<> onReceived(std::string data) override {
        std::cout << "Received: " << data << std::endl;
        NetworkCommand cmd = serializer->deserialize<NetworkCommand>(data);
        for (const auto &callback : callbacks)
            co_await callback(cmd);
    }
};
