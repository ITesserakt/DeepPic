//
// Created by tesserakt on 08.12.2021.
//

#pragma once

#include <QGraphicsScene>
#include <cppcoro/io_service.hpp>

#include "CommandExecutor.h"
#include "Commands.h"
#include "command/NetworkCommand.h"

class UICommandExecutor : public CommandExecutor {
private:
    QGraphicsScene *scene_;
    cppcoro::io_service uiScheduler_;

public:
    UICommandExecutor(QGraphicsScene &scene) : scene_(&scene) {}

    cppcoro::task<> execute(Command &cmd) {
        Command &actualCommand = cmd;
        if (dynamic_cast<NetworkCommand *>(&cmd) != nullptr) {
            const auto &network = *dynamic_cast<NetworkCommand *>(&cmd);
            actualCommand = network.inner<Command>();
        }
        if (dynamic_cast<command::UI *>(&actualCommand) != nullptr) {
            auto &ui = *dynamic_cast<command::UI *>(&cmd);
            ui.scene = scene_;
        }

        cppcoro::schedule_on(uiScheduler_);
        actualCommand.execute();
        co_return;
    }

    cppcoro::task<> rollback(RCommand &cmd) {
        RCommand &actualCommand = cmd;
        if (dynamic_cast<NetworkCommand *>(&cmd) != nullptr) {
            const auto &network = *dynamic_cast<NetworkCommand *>(&cmd);
            actualCommand = network.inner<RCommand>();
        }
        if (dynamic_cast<command::UI *>(&actualCommand) != nullptr) {
            auto &ui = *dynamic_cast<command::UI *>(&actualCommand);
            ui.scene = scene_;
        }

        cppcoro::schedule_on(uiScheduler_);
        actualCommand.rollback();
        co_return;
    }
};