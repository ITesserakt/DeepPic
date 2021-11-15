//
// Created by tesserakt on 30.10.2021.
//

#include <cppcoro/cancellation_source.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/when_all.hpp>
#include <iostream>

#include "Server.h"

class ConsoleServer : public Server<BoostSerializer> {
private:
    std::vector<shared_socket> clients;

public:
    cppcoro::task<> shutdown() override {
        for (auto &client : clients) {
            client->close_recv();
            client->close_send();
            co_await client->disconnect();
        }
    }

protected:
    cppcoro::task<> broadcast(std::string data) override {
        std::cout << "Broadcasting: " << data << std::endl;
        co_return;
    }

    void clientAttached(shared_socket client) override {
        std::cout << "New client attached: " << client->native_handle()
                  << ", from: " << client->remote_endpoint().to_string() << std::endl;
        clients.emplace_back(client);
    }

    void onReceived(std::string data) override {
        std::cout << "Received: " << data << std::endl;
        if (data.starts_with("shutdown"))
            source.request_cancellation();
    }
};

int main(int argc, char **argv) {
    ConsoleServer server;
    auto localhost = cppcoro::net::ipv4_address::from_string(argv[1]).value();
    auto endpoint = cppcoro::net::ipv4_endpoint(localhost, atoi(argv[2]));

    server.createSerializer();
    cppcoro::sync_wait(cppcoro::when_all(
            server.start(endpoint),
            server.listen()));
    cppcoro::sync_wait(server.shutdown());
}