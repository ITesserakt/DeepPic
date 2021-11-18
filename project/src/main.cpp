//
// Created by tesserakt on 30.10.2021.
//

#include <cppcoro/cancellation_source.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/when_all.hpp>
#include <cppcoro/when_all_ready.hpp>
#include <iostream>

#include "CommandBus.h"
#include "HelloCommand.h"
#include "LocalCommandExecutor.h"
#include "NetworkCommandExecutor.h"
#include "Server.h"

class ConsoleServer : public Server<BoostSerializer> {
private:
    std::vector<shared_socket> clients;
    std::vector<std::function<cppcoro::task<>(std::string)>> callbacks;

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
    cppcoro::task<> broadcast(std::string data) override {
        std::cout << "Broadcasting: " << data << std::endl;
        cppcoro::async_scope scope;
        for (auto &client : clients)
            scope.spawn([&]() -> cppcoro::task<> {
                co_await client->send(data.data(), data.size(), source.token());
            }());
        co_await scope.join();
    }

    void clientAttached(shared_socket client) override {
        std::cout << "New client attached: " << client->native_handle()
                  << ", from: " << client->remote_endpoint().to_string() << std::endl;
        clients.emplace_back(client);
    }

    cppcoro::task<> onReceived(std::string data) override {
        std::cout << "Received: " << data << std::endl;
        for (const auto &callback : callbacks)
            co_await callback(data);
    }
};

int main(int argc, char **argv) {
}