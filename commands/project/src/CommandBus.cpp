//
// Created by tesserakt on 31.10.2021.
//
#include "CommandBus.h"

boost::asio::awaitable<void> CommandBus::execute(Command &&command) {
    for (auto &executor : executors) {
        co_await executor.execute(command);
    }
}

boost::asio::awaitable<void> CommandBus::rollback(RCommand &&command) {
    for (auto &executor : executors) {
        co_await executor.rollback(command);
    }
}
