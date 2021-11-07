//
// Created by tesserakt on 01.11.2021.
//

#include "NetworkCommandExecutor.h"

NetworkCommand NetworkCommandExecutor::wrapCommand(Command &&command) {
    auto network = dynamic_cast<NetworkCommand *>(&command);
    if (network == nullptr) {
        return NetworkCommand(server->id(), &command);
    }
    return std::move(*network);
}

cppcoro::task<void> NetworkCommandExecutor::execute(Command &&cmd) {
    auto wrapped = wrapCommand(std::move(cmd));
    co_await server->broadcast(std::move(wrapped));
}

cppcoro::task<void> NetworkCommandExecutor::rollback(RCommand &&cmd) {
    auto wrapped = wrapCommand(std::move(cmd));
    co_await server->broadcast(std::move(wrapped));
}

NetworkCommandExecutor::NetworkCommandExecutor(std::shared_ptr<Server<Serializer>> server) : server(server) {}
