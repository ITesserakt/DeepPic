//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <memory>

#include "CommandExecutor.h"
#include "Transporter.h"
#include "BoostSerializer.h"
#include "command/NetworkCommand.h"

class NetworkCommandExecutor : public CommandExecutor {
public:
    using Serializer = BoostSerializer;
    using TransporterType = Transporter<std::istringstream, Serializer>;

private:
    std::shared_ptr<TransporterType> transporter;

    NetworkCommand wrapCommand(Command &&command);

public:
    cppcoro::task<void> execute(Command &&cmd) override;
    cppcoro::task<void> rollback(RCommand &&cmd) override;

    explicit NetworkCommandExecutor(std::shared_ptr<TransporterType> server);
};