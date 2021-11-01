//
// Created by tesserakt on 30.10.2021.
//

#include <cppcoro/sync_wait.hpp>

#include "CommandBus.h"
#include "LocalCommandExecutor.h"
#include "command/Command.h"
#include "command/NetworkCommand.h"

struct HelloCommand : public RCommand {
    void execute() override {
        std::cout << "Hello!" << std::endl;
    }

    void rollback() override {
        std::cout << "!olleH" << std::endl;
    }
};

int main() {
    auto cmd = HelloCommand();
    auto bus = CommandBus();

    bus.addExecutor<LocalCommandExecutor>();
    cppcoro::sync_wait(bus.execute(std::move(cmd)));
    return 0;
}