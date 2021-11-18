//
// Created by tesserakt on 18.11.2021.
//

#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <exception>

#include "Serializer.h"
#include "command/Command.h"

class HelloCommand : public Command {
private:
    friend class boost::serialization::access;

    void serialize(auto &ar, const unsigned int) {
        ar &boost::serialization::base_object<Command>(*this) & dummy;
    }

    int dummy;

public:
    void execute() override { throw std::runtime_error("test"); }

    explicit HelloCommand(int dummy) : dummy(dummy) {}

    int getDummy() const { return dummy; }

    bool operator==(const HelloCommand &rhs) const {
        return dummy == rhs.dummy;
    }
    bool operator!=(const HelloCommand &Rhs) const {
        return !(Rhs == *this);
    }

    constexpr const char *type() const override {
        return "HelloCommand";
    }
};

namespace boost::serialization {
    void load_construct_data(auto &ar, HelloCommand *p, const unsigned int) {
        int d;
        ar &d;
        ::new (p) HelloCommand(d);
    }
}// namespace boost::serialization

static_assert(Serializable<HelloCommand, boost::archive::text_iarchive>);

JSONCONS_ALL_CTOR_GETTER_NAME_TRAITS(
        HelloCommand,
        (type, "type", JSONCONS_RDONLY, [](const char *type) { return type == std::string("HelloCommand"); }),
        (getDummy, "dummy"))