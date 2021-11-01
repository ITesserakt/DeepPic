//
// Created by tesserakt on 01.11.2021.
//
#include "LocalCommandExecutor.h"

cppcoro::task<void> LocalCommandExecutor::execute(Command &&cmd) {
    co_return cmd.execute();
}

cppcoro::task<> LocalCommandExecutor::rollback(RCommand &&cmd) {
    co_return cmd.rollback();
}