//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

class Command {
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int) {
    }

public:
    virtual void execute() = 0;

    virtual ~Command() = default;
};

class RCommand : public Command {
private:
    friend class boost::serialization::access;

    void serialize(auto &ar, const unsigned int) {
        ar &boost::serialization::base_object<Command>(*this);
    }

public:
    virtual void rollback() = 0;
};

template <typename T>
concept Rollback = requires(T &a) {
    a.rollback();
};

BOOST_CLASS_EXPORT_KEY(Command);
BOOST_CLASS_EXPORT_KEY(RCommand);