//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include "Command.h"
#include <boost/asio/awaitable.hpp>

class CommandExecutor {
public:
    virtual boost::asio::awaitable<void> execute(Command &cmd) = 0;
    virtual boost::asio::awaitable<void> rollback(RCommand &cmd) = 0;
};