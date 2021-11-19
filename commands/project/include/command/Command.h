//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <jsoncons/json.hpp>

#include "JsonPolymorphic.h"

struct Command {
    virtual void execute() = 0;

    virtual ~Command() = default;
};

struct RCommand : Command {
    virtual void rollback() = 0;

private:
    JSONCONS_POLYMORPHIC_INIT_DEFINES(RCommand);
};