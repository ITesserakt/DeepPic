//
// Created by tesserakt on 03.11.2021.
//

#include "command/NetworkCommand.h"

BOOST_CLASS_EXPORT_IMPLEMENT(NetworkCommand)

void NetworkCommand::rollback() {
    auto rCommand = dynamic_cast<RCommand *>(command);
    if (rCommand != nullptr)
        rCommand->rollback();
}

NetworkCommand::NetworkCommand(unsigned int clientId, Command *command)
    : clientId(clientId), command(command) {}

void NetworkCommand::execute() {
    command->execute();
}

NetworkCommand::~NetworkCommand() {
    delete command;
}
