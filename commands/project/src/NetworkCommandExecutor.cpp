//
// Created by tesserakt on 01.11.2021.
//

#include <utility>

#include "NetworkCommandExecutor.h"

NetworkCommand NetworkCommandExecutor::wrapCommand(Command &command) {
    auto network = dynamic_cast<NetworkCommand *>(&command);
    if (network == nullptr) {
        return {transporter->id(), &command};
    }
    return std::move(*network);
}

cppcoro::task<void> NetworkCommandExecutor::execute(Command &cmd) {
    auto wrapped = wrapCommand(cmd);
    co_await transporter->send(wrapped);
}

cppcoro::task<void> NetworkCommandExecutor::rollback(RCommand &cmd) {
    auto wrapped = wrapCommand(cmd);
    co_await transporter->send(wrapped);
}

NetworkCommandExecutor::NetworkCommandExecutor(std::shared_ptr<TransporterType> server) : transporter(std::move(server)) {}
