//
// Created by tesserakt on 31.10.2021.
//
#include <cppcoro/async_scope.hpp>
#include <cppcoro/operation_cancelled.hpp>
#include <cppcoro/schedule_on.hpp>

#include "CommandBus.h"

cppcoro::task<> CommandBus::execute(Command &&command) {
    cppcoro::schedule_on(thread_pool);
    for (auto &executor : executors)
        co_await executor->execute(std::forward<Command>(command));
}

cppcoro::task<> CommandBus::rollback(RCommand &&command) {
    cppcoro::schedule_on(thread_pool);
    for (auto &executor : executors)
        co_await executor->rollback(std::forward<RCommand>(command));
}