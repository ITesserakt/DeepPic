//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <iostream>

#include "Command.h"

namespace __detail {
    template <std::derived_from<Command> C>
    class NetworkCommandBase {
    protected:
        unsigned int clientId;
        C command;

    public:
        void execute() {
            std::cout << "Running from remote: " << std::endl;
            this->command.execute();
        }

        NetworkCommandBase(unsigned int clientId, C &&command) : clientId(clientId), command(command) {}

        [[nodiscard]] C inner() const { return command; }
    };
}// namespace __detail

template <typename C>
class NetworkCommand : public __detail::NetworkCommandBase<C>, public Command {
public:
    void execute() override {
        this->__detail::NetworkCommandBase<C>::execute();
    }

    NetworkCommand(unsigned int clientId, C &&command) : __detail::NetworkCommandBase<C>(clientId, std::forward<C>(command)) {}
};

template <Rollback C>
class NetworkCommand<C> : public __detail::NetworkCommandBase<C>, public RCommand {
public:
    void rollback() override {
        std::cout << "Running from remote: " << std::endl;
        this->command.rollback();
    }

    void execute() override {
        this->__detail::NetworkCommandBase<C>::execute();
    }

    NetworkCommand(unsigned int clientId, C &&command) : __detail::NetworkCommandBase<C>(clientId, std::forward<C>(command)) {}
};