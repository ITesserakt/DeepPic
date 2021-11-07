//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <memory>

#include "CommandExecutor.h"
#include "Server.h"
#include "command/NetworkCommand.h"

class NetworkCommandExecutor : public CommandExecutor {
public:
    using Serializer = BoostSerializer;

private:
    std::shared_ptr<Server<Serializer>> server;

    NetworkCommand wrapCommand(Command &&command);

public:
    cppcoro::task<void> execute(Command &&cmd) override;
    cppcoro::task<void> rollback(RCommand &&cmd) override;

    explicit NetworkCommandExecutor(std::shared_ptr<Server<Serializer>> server);
};