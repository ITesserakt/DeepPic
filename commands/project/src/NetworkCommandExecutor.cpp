//
// Created by tesserakt on 01.11.2021.
//

#include "NetworkCommandExecutor.h"

NetworkCommand<Command> NetworkCommandExecutor::wrapCommand(Command &&command) {
    auto network = dynamic_cast<NetworkCommand<Command> *>(&command);
    if (network == nullptr)
        return NetworkCommand(server->id(), &command);
    return std::move(*network);
}

NetworkCommand<RCommand> NetworkCommandExecutor::wrapCommand(RCommand &&command) {
    auto network = dynamic_cast<NetworkCommand<RCommand> *>(&command);
    if (network == nullptr)
        return NetworkCommand(server->id(), &command);
    return std::move(*network);
}

cppcoro::task<void> NetworkCommandExecutor::execute(Command &&cmd) {
    auto wrapped = wrapCommand(std::forward<Command>(cmd));
    wrapped.execute();
    co_await server->broadcast(std::move(wrapped));
}

cppcoro::task<void> NetworkCommandExecutor::rollback(RCommand &&cmd) {
    auto wrapped = wrapCommand(std::forward<RCommand>(cmd));
    wrapped.rollback();
    co_await server->broadcast(std::move(wrapped));
}

NetworkCommandExecutor::NetworkCommandExecutor(std::shared_ptr<Server> server) : server(server) {}
