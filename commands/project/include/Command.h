//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <concepts>

template <typename T>
concept Rollback = requires(T a) {
    a.rollback();
};

class Command {
public:
    virtual void execute() = 0;
};

class RCommand : Command {
public:
    virtual void rollback() = 0;
};
