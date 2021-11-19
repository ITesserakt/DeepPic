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

JSONCONS_POLYMORPHIC_DEFINES_IMPLEMENT(NetworkCommand) {
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json>::definitions
            .addIsImpl([](const auto &json) { return json.template is<NetworkCommand>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<NetworkCommand>()) return std::make_shared<NetworkCommand>(json.template as<NetworkCommand>());
                return std::shared_ptr<NetworkCommand>();
            })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = std::dynamic_pointer_cast<NetworkCommand>(ptr)) return json(*p);
                return jsoncons::json::null();
            });

    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json, std::unique_ptr<RCommand>>::definitions
            .addIsImpl([](const auto &json) { return json.template is<NetworkCommand>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<NetworkCommand>()) return std::make_unique<NetworkCommand>(json.template as<NetworkCommand>());
                return std::unique_ptr<NetworkCommand>();
            })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = dynamic_cast<NetworkCommand *>(ptr)) return jsoncons::json(*p);
                return jsoncons::json::null();
            });
}