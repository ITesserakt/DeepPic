//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <jsoncons/json.hpp>

struct Typeable {
    [[nodiscard]] constexpr virtual const char *type() const = 0;
};

class Command : public Typeable {
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

namespace jsoncons {
    template <typename Json>
    struct json_type_traits<Json, std::shared_ptr<RCommand>> {
        static bool is(const Json &) noexcept;
        static std::shared_ptr<RCommand> as(const Json &);
        static Json to_json(const std::shared_ptr<RCommand> &);
    };

    template <typename Json>
    struct json_type_traits<Json, std::unique_ptr<RCommand>> {
        static bool is(const Json &) noexcept;
        static std::unique_ptr<RCommand> as(const Json &);
        static Json to_json(const std::unique_ptr<RCommand> &);
    };

    template <typename Json>
    struct json_type_traits<Json, std::shared_ptr<Command>> {
        static bool is(const Json &) noexcept;
        static std::shared_ptr<Command> as(const Json &);
        static Json to_json(const std::shared_ptr<Command> &);
    };

    template <typename Json>
    struct json_type_traits<Json, std::unique_ptr<Command>> {
        static bool is(const Json &) noexcept;
        static std::unique_ptr<Command> as(const Json &);
        static Json to_json(const std::unique_ptr<Command> &);
    };
}// namespace jsoncons