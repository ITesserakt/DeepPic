//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <memory>

#include "CommandExecutor.h"
#include "Server.h"
#include "command/NetworkCommand.h"

class NetworkCommandExecutor : public CommandExecutor {
private:
    std::shared_ptr<Server> server;

    NetworkCommand<Command> wrapCommand(Command &&command);
    NetworkCommand<RCommand> wrapCommand(RCommand &&command);

public:
    cppcoro::task<void> execute(Command &&cmd) override;
    cppcoro::task<void> rollback(RCommand &&cmd) override;

    explicit NetworkCommandExecutor(std::shared_ptr<Server> server);
};