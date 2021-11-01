//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include <cppcoro/static_thread_pool.hpp>
#include <memory>
#include <vector>

#include "CommandExecutor.h"

class CommandBus {
private:
    cppcoro::static_thread_pool thread_pool;
    std::vector<std::unique_ptr<CommandExecutor>> executors;

public:
    explicit CommandBus(unsigned int threadsCount = 1) : thread_pool(threadsCount) {}

    cppcoro::task<> execute(Command &&command);

    cppcoro::task<> rollback(RCommand &&command);

    template <std::derived_from<CommandExecutor> T, typename... Args>
    void addExecutor(Args &&...args) {
        executors.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
};