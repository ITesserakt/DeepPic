//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <iostream>
#include <memory>

#include "Command.h"
#include "JsonPolymorphic.h"

class NetworkCommand : public RCommand, public jsoncons::polymorphic::TypeableGetter<NetworkCommand> {
private:
    NetworkCommand(unsigned int clientId, jsoncons::json command)
        : clientId(clientId), command(command) {}

    [[nodiscard]] auto getClientId() const { return clientId; }
    [[nodiscard]] auto &getCommand() const { return command; }
    JSONCONS_TYPE_TRAITS_FRIEND;

    unsigned int clientId;
    jsoncons::json command;

public:
    void execute() override;

    NetworkCommand(unsigned int clientId, std::unique_ptr<Command> command);

    template <typename T>
    [[nodiscard]] T inner() const { return std::move(command.as<T>()); }

    void rollback() override;

    bool isRollback() const { return false; }

    unsigned int transporterId() const { return clientId; }
};

JSONCONS_ALL_CTOR_GETTER_NAME_TRAITS(
        NetworkCommand,
        JSONCONS_POLYMORPHIC_TYPE_VAL(NetworkCommand),
        (getClientId, "client_id"),
        (getCommand, "command"))
