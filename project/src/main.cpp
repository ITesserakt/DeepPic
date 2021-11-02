//
// Created by tesserakt on 30.10.2021.
//

#include <cppcoro/sync_wait.hpp>

#include "CommandBus.h"
#include "LocalCommandExecutor.h"
#include "command/Command.h"
#include "command/NetworkCommand.h"
#include "NetworkCommandExecutor.h"

struct HelloCommand : public RCommand {
    int aboba = 100500;

    void execute() override {
        std::cout << "Hello!" << std::endl;
    }

    void rollback() override {
        std::cout << "!olleH" << std::endl;
    }
};

struct NoopServer : Server {
    unsigned int id() override {
        return 0;
    }

    cppcoro::task<> broadcast(std::any any) override {
        co_return;
    }
};

int main() {
    auto cmd = NetworkCommand(0, new HelloCommand());
    auto bus = CommandBus();
    auto server = std::make_shared<NoopServer>();

    bus.addExecutor<LocalCommandExecutor>();
    bus.addExecutor<NetworkCommandExecutor>(server);
    cppcoro::sync_wait(bus.execute(std::move(cmd)));
    return 0;
}