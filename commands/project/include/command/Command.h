//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <jsoncons/json.hpp>

#include "JsonSerializer.h"
#include "JsonExtensions.h"

struct Command {
    virtual void execute() = 0;

    virtual ~Command() = default;
};

struct RCommand : Command {
    virtual void rollback() = 0;
};