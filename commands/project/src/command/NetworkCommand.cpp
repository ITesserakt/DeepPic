//
// Created by tesserakt on 03.11.2021.
//

#include "command/NetworkCommand.h"

void NetworkCommand::rollback() {
    auto rCommand = dynamic_cast<RCommand *>(command.get());
    if (rCommand != nullptr) {
        command.release();
        rCommand->rollback();
    }
}

NetworkCommand::NetworkCommand(unsigned int clientId, Command *command)
    : clientId(clientId), command(command) {}

void NetworkCommand::execute() {
    command->execute();
}