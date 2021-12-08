//
// Created by tesserakt on 31.10.2021.
//

#pragma once

#include "cppcoro/static_thread_pool.hpp"
#include <cppcoro/schedule_on.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/when_all.hpp>
#include <memory>
#include <vector>

#include "CommandExecutor.h"
#include "cppcoro/task.hpp"
#include "cppcoro/when_all_ready.hpp"

template <bool blocking>
class CommandBus {
private:
    cppcoro::static_thread_pool thread_pool;
    std::vector<std::unique_ptr<CommandExecutor>> executors;

public:
    using BlockingCall = typename std::conditional<blocking, void, cppcoro::task<>>::type;

    explicit CommandBus(unsigned int threadsCount = 1) : thread_pool(threadsCount) {}

    BlockingCall execute(Command &command) {
        cppcoro::schedule_on(thread_pool);
        std::vector<cppcoro::task<>> tasks;
        for (auto &executor : executors)
            tasks.emplace_back(executor->execute(command));
        auto result = cppcoro::sync_wait(cppcoro::when_all_ready(std::move(tasks)));

        std::exception_ptr ex;
        for (auto &solution : result) {
            try {
                solution.result();
            } catch (...) {
                ex = std::current_exception();
            }
        }
        if (ex)
            rethrow_exception(ex);
    }

    BlockingCall rollback(RCommand &command) {
        cppcoro::schedule_on(thread_pool);
        std::vector<cppcoro::task<>> tasks;
        for (auto &executor : executors)
            tasks.emplace_back(executor->rollback(command));
        auto result = cppcoro::sync_wait(cppcoro::when_all_ready(std::move(tasks)));

        std::exception_ptr ex;
        for (auto &solution : result) {
            try {
                solution.result();
            } catch (...) {
                ex = std::current_exception();
            }
        }
        if (ex)
            rethrow_exception(ex);
    }

    template <std::derived_from<CommandExecutor> T, typename... Args>
    T &addExecutor(Args &&...args) {
        auto executor = std::make_unique<T>(std::forward<Args>(args)...);
        auto &ref = *executor;
        executors.emplace_back(std::move(executor));
        return ref;
    }
};