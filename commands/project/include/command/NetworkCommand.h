//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <iostream>
#include <memory>

#include "Command.h"

class NetworkCommand : public RCommand, public jsoncons::polymorphic::TypeableGetter<NetworkCommand> {
private:
    NetworkCommand(unsigned int clientId, std::unique_ptr<Command> command)
        : clientId(clientId), command(std::move(command)) {}

    [[nodiscard]] auto getClientId() const { return clientId; }
    [[nodiscard]] auto &getCommand() const { return command; }
    JSONCONS_TYPE_TRAITS_FRIEND;

    unsigned int clientId;
    std::unique_ptr<Command> command;

    JSONCONS_POLYMORPHIC_INIT_DEFINES(NetworkCommand);

public:
    void execute() override;

    NetworkCommand(unsigned int clientId, Command *command);

    template <typename T>
    requires std::is_base_of_v<Command, T>
    [[nodiscard]] T &inner() const { return *dynamic_cast<T *>(command.get()); }

    void rollback() override;
};

JSONCONS_ALL_CTOR_GETTER_NAME_TRAITS(
        NetworkCommand,
        JSONCONS_POLYMORPHIC_TYPE_VAL(NetworkCommand),
        (getClientId, "client_id"),
        (getCommand, "command"))
