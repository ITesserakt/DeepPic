//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <cppcoro/task.hpp>

#include "CommandExecutor.h"

class LocalCommandExecutor : public CommandExecutor {
public:
    cppcoro::task<void> execute(Command &&cmd) override;

    cppcoro::task<void> rollback(RCommand &&cmd) override;
};