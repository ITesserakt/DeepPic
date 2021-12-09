//
// Created by tesserakt on 03.11.2021.
//

#include "command/NetworkCommand.h"

void NetworkCommand::rollback() {
    if (isRollback()) {
        inner<std::unique_ptr<RCommand>>()->rollback();
    }
}

NetworkCommand::NetworkCommand(unsigned int clientId, std::unique_ptr<Command> command)
    : clientId(clientId), command(command) {}

void NetworkCommand::execute() {
    inner<std::unique_ptr<Command>>()->execute();
}