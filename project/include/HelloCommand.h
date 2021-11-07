//
// Created by tesserakt on 03.11.2021.
//

#pragma once

#include "command/Command.h"

class HelloCommand : public Command {
private:
    friend class boost::serialization::access;

    void serialize(auto &ar, const unsigned int) {
        ar &boost::serialization::base_object<Command>(*this);
    }

public:
    void execute() override;
};

BOOST_CLASS_EXPORT_KEY(HelloCommand)