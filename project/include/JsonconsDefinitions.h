//
// Created by tesserakt on 15.11.2021.
//

#pragma once

#include "command/Command.h"
#include "command/NetworkCommand.h"

namespace jsoncons {
    template <typename Json>
    bool json_type_traits<Json, std::shared_ptr<RCommand>>::is(const Json &ajson) noexcept {
        if (!ajson.is_object()) return false;
        JSONCONS_POLYMORPHIC_IS(RCommand, 0, 0, NetworkCommand, 0)
        return false;
    }

    template <typename Json>
    std::shared_ptr<RCommand> json_type_traits<Json, std::shared_ptr<RCommand>>::as(const Json &ajson) {
        if (!ajson.is_object()) return {};
        JSONCONS_POLYMORPHIC_AS(RCommand, 0, 0, NetworkCommand, 0)
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::shared_ptr<RCommand>>::to_json(const std::shared_ptr<RCommand> &ptr) {
        if (ptr == nullptr) return Json::null();
        JSONCONS_POLYMORPHIC_TO_JSON(RCommand, 0, 0, NetworkCommand, 0)
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::unique_ptr<RCommand>>::is(const Json &ajson) noexcept {
        if (!ajson.is_object()) return false;
        JSONCONS_POLYMORPHIC_IS(RCommand, 0, 0, NetworkCommand, 0)
        return false;
    }

    template <typename Json>
    std::unique_ptr<RCommand> json_type_traits<Json, std::unique_ptr<RCommand>>::as(const Json &ajson) {
        if (!ajson.is_object()) return {};
        JSONCONS_POLYMORPHIC_AS(RCommand, 0, 0, NetworkCommand, 0)
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::unique_ptr<RCommand>>::to_json(const std::unique_ptr<RCommand> &ptr) {
        if (ptr == nullptr) return Json::null();
        JSONCONS_POLYMORPHIC_TO_JSON(RCommand, 0, 0, NetworkCommand, 0)
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::shared_ptr<Command>>::is(const Json &ajson) noexcept {
        if (!ajson.is_object()) return false;
        JSONCONS_POLYMORPHIC_IS(Command, 0, 0, std::shared_ptr<RCommand>, 0)
        return false;
    }

    template <typename Json>
    std::shared_ptr<Command> json_type_traits<Json, std::shared_ptr<Command>>::as(const Json &ajson) {
        if (!ajson.is_object()) return {};
        if (ajson.template is<std::shared_ptr<RCommand>>()) return ajson.template as<std::shared_ptr<RCommand>>();
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::shared_ptr<Command>>::to_json(const std::shared_ptr<Command> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = std::dynamic_pointer_cast<RCommand>(ptr)) return Json(p);
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::unique_ptr<Command>>::is(const Json &ajson) noexcept {
        if (!ajson.is_object()) return false;
        JSONCONS_POLYMORPHIC_IS(Command, 0, 0, std::shared_ptr<RCommand>, 0)
        return false;
    }

    template <typename Json>
    std::unique_ptr<Command> json_type_traits<Json, std::unique_ptr<Command>>::as(const Json &ajson) {
        if (!ajson.is_object()) return {};
        if (ajson.template is<RCommand>()) return ajson.template as<std::unique_ptr<RCommand>>();
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::unique_ptr<Command>>::to_json(const std::unique_ptr<Command> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = dynamic_cast<RCommand *>(ptr.get())) return Json(std::unique_ptr<RCommand>(p));
        return Json::null();
    }
}// namespace jsoncons
