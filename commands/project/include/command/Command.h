//
// Created by tesserakt on 30.10.2021.
//

#pragma once

#include <jsoncons/json.hpp>

#include "JsonSerializer.h"

struct Command {
    virtual void execute() = 0;

    virtual ~Command() = default;
};

struct RCommand : Command {
    virtual void rollback() = 0;
};

template <typename T>
concept Rollback = requires(T &a) {
    a.rollback();
};

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