//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include <vector>

#include "CommandExecutor.h"

class CommandBus {
private:
    std::vector<CommandExecutor> executors;

public:
    boost::asio::awaitable<void> execute(Command &&command);

    boost::asio::awaitable<void> rollback(RCommand &&command);
};