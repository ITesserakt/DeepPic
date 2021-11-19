//
// Created by tesserakt on 18.11.2021.
//

#pragma once

#include <exception>

#include "Serializer.h"
#include "command/Command.h"

class HelloCommand : public Command, public jsoncons::polymorphic::TypeableMember<HelloCommand> {
private:
    JSONCONS_TYPE_TRAITS_FRIEND;
    JSONCONS_POLYMORPHIC_INIT_DEFINES(HelloCommand);

    int dummy;

public:
    void execute() override { throw std::runtime_error("test"); }

    explicit HelloCommand(int dummy = 0) : dummy(dummy) {}

    int getDummy() const { return dummy; }

    bool operator==(const HelloCommand &rhs) const {
        return dummy == rhs.dummy;
    }
    bool operator!=(const HelloCommand &Rhs) const {
        return !(Rhs == *this);
    }
};

JSONCONS_POLYMORPHIC_DEFINES_IMPLEMENT(HelloCommand) {
    jsoncons::polymorphic::Definitions<Command, jsoncons::json>::definitions
            .addIsImpl([](const auto &json) { return json.template is<HelloCommand>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<HelloCommand>()) return std::make_shared<HelloCommand>(json.template as<HelloCommand>());
                return std::shared_ptr<HelloCommand>();
            })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = std::dynamic_pointer_cast<HelloCommand>(ptr)) return json(*p);
                return jsoncons::json::null(); });

    jsoncons::polymorphic::Definitions<Command, jsoncons::json, std::unique_ptr<Command>>::definitions
            .addIsImpl([](const auto &json) { return json.template is<HelloCommand>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<HelloCommand>()) return std::make_unique<HelloCommand>(json.template as<HelloCommand>());
                return std::unique_ptr<HelloCommand>();
            })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = dynamic_cast<HelloCommand*>(ptr)) return jsoncons::json(*p);
                return jsoncons::json::null(); });
}

JSONCONS_ALL_MEMBER_NAMED_TRAITS(
        HelloCommand,
        JSONCONS_POLYMORPHIC_TYPE_VAL(HelloCommand),
        (dummy, "dummy"))
