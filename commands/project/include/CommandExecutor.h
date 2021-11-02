//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include "command/Command.h"
#include "cppcoro/task.hpp"

class CommandExecutor {
public:
    virtual cppcoro::task<void> execute(Command &&cmd) = 0;
    virtual cppcoro::task<void> rollback(RCommand &&cmd) = 0;

    virtual ~CommandExecutor() = default;
};