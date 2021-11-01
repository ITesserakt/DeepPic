//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <concepts>

class Command {
public:
    virtual void execute() = 0;
};

class RCommand : public Command {
public:
    virtual void rollback() = 0;
};

template <typename T>
concept Rollback = requires(T a) {
    a.rollback();
};
