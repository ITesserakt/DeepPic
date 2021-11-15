//
// Created by tesserakt on 31.10.2021.
//
#include <cppcoro/schedule_on.hpp>

#include "CommandBus.h"

void CommandBus::execute(Command &&command) {
    for (auto &executor : executors) {
        async.spawn(
                executor->execute(std::forward<Command>(command)) |
                cppcoro::schedule_on(thread_pool));
    }
}

void CommandBus::rollback(RCommand &&command) {
    for (auto &executor : executors) {
        async.spawn(
                executor->rollback(std::forward<RCommand>(command)) |
                cppcoro::schedule_on(thread_pool));
    }
}