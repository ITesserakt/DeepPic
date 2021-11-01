//
// Created by tesserakt on 31.10.2021.
//
#include "CommandBus.h"

cppcoro::task<> CommandBus::execute(Command &&command) {
    co_await thread_pool.schedule();
    for (auto &executor : executors) {
        co_await executor->execute(command);
    }
}

cppcoro::task<> CommandBus::rollback(RCommand &&command) {
    co_await thread_pool.schedule();
    for (auto &executor : executors) {
        co_await executor->rollback(command);
    }
}