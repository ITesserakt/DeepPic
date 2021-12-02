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

JSONCONS_ALL_MEMBER_NAMED_TRAITS(
        HelloCommand,
        JSONCONS_POLYMORPHIC_TYPE_VAL(HelloCommand),
        (dummy, "dummy"))
