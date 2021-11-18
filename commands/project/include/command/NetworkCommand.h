//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/nvp.hpp>
#include <iostream>
#include <memory>

#include "Command.h"

class NetworkCommand : public RCommand {
private:
    friend class boost::serialization::access;

    void serialize(auto &ar, const unsigned int) {
        ar &boost::serialization::base_object<RCommand>(*this) & BOOST_SERIALIZATION_NVP(clientId) & BOOST_SERIALIZATION_NVP(command);
    }

    NetworkCommand(unsigned int clientId, std::unique_ptr<Command> command)
        : clientId(clientId), command(std::move(command)) {}
    [[nodiscard]] auto getClientId() const { return clientId; }
    [[nodiscard]] auto &getCommand() const { return command; }
    JSONCONS_TYPE_TRAITS_FRIEND;

protected:
    unsigned int clientId;
    std::unique_ptr<Command> command;

public:
    void execute() override;

    NetworkCommand(unsigned int clientId, Command *command);

    template <typename T>
    requires std::is_base_of_v<Command, T>
    [[nodiscard]] T &inner() const { return *dynamic_cast<T *>(command.get()); }

    void rollback() override;

    [[nodiscard]] constexpr const char *type() const override;
};

namespace boost::serialization {
    void save_construct_data(auto &ar, const NetworkCommand *data, const unsigned int) {
    }

    void load_construct_data(auto &ar, NetworkCommand *data, const unsigned int) {
        unsigned int clientId;
        Command *cmd;
        ar &clientId &cmd;
        ::new (data) NetworkCommand(clientId, cmd);
    }
}// namespace boost::serialization

BOOST_CLASS_EXPORT_KEY(NetworkCommand)
JSONCONS_ALL_CTOR_GETTER_NAME_TRAITS(
        NetworkCommand,
        (type, "type", JSONCONS_RDONLY, [](const char *type) { return type == std::string("NetworkCommand"); }),
        (getClientId, "client_id"),
        (getCommand, "command"))