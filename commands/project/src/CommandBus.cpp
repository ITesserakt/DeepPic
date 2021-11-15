//
// Created by tesserakt on 31.10.2021.
//
#include <cppcoro/schedule_on.hpp>
#include <cppcoro/async_scope.hpp>

#include "CommandBus.h"

cppcoro::task<> CommandBus::execute(Command &&command) {
    cppcoro::async_scope async;
    for (auto &executor : executors) {
        async.spawn(
                executor->execute(std::forward<Command>(command)) |
                cppcoro::schedule_on(thread_pool));
    }
    co_await async.join();
}

cppcoro::task<> CommandBus::rollback(RCommand &&command) {
    cppcoro::async_scope async;
    for (auto &executor : executors) {
        async.spawn(
                executor->rollback(std::forward<RCommand>(command)) |
                cppcoro::schedule_on(thread_pool));
    }
    co_await async.join();
}