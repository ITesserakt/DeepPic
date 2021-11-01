//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include <cppcoro/static_thread_pool.hpp>
#include <memory>
#include <vector>

#include "CommandExecutor.h"

class CommandBus : public CommandExecutor {
private:
    cppcoro::static_thread_pool thread_pool;
    std::vector<std::unique_ptr<CommandExecutor>> executors;

public:
    explicit CommandBus(unsigned int threadsCount = 1) : thread_pool(threadsCount) {}

    cppcoro::task<> execute(Command &&command) override;

    cppcoro::task<> rollback(RCommand &&command) override;

    template <std::derived_from<CommandExecutor> T, typename... Args>
    T &addExecutor(Args &&...args) {
        auto executor = std::make_unique<T>(std::forward<Args>(args)...);
        auto &ref = *executor;
        executors.emplace_back(std::move(executor));
        return ref;
    }
};