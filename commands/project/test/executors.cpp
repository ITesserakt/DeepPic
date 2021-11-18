//
// Created by potry on 11/18/2021.
//

#include <cppcoro/sync_wait.hpp>
#include <gtest/gtest.h>

#include "CommandBus.h"
#include "LocalCommandExecutor.h"
#include "HelloCommand.h"

TEST(executors, executing_command_through_local_command_executor) {
    auto executor = LocalCommandExecutor();
    auto cmd = HelloCommand(5);

    EXPECT_THROW(cppcoro::sync_wait(executor.execute(std::move(cmd))), std::runtime_error);
}

TEST(executors, executing_command_through_network_command_executor) {
    // create network command executor, server and client
    // expect that client NCE accepts command and throws an exception
}

TEST(executors, executing_command_through_command_bus) {
    auto bus = CommandBus();
    auto cmd = HelloCommand(5);

    bus.addExecutor<LocalCommandExecutor>();

    EXPECT_THROW(cppcoro::sync_wait(bus.execute(std::move(cmd))), std::runtime_error);
}