//
// Created by tesserakt on 15.11.2021.
//

#pragma once

#include "command/Command.h"
#include "command/NetworkCommand.h"

namespace jsoncons {
    template <typename Json>
    bool json_type_traits<Json, std::shared_ptr<RCommand>>::is(const Json &json) noexcept {
        if (!json.is_object()) return false;
        if (json.template is<NetworkCommand>()) return true;
        return false;
    }

    template <typename Json>
    std::shared_ptr<RCommand> json_type_traits<Json, std::shared_ptr<RCommand>>::as(const Json &json) {
        if (!json.is_object()) return {};
        if (json.template is<NetworkCommand>()) return std::make_shared<NetworkCommand>(json.template as<NetworkCommand>());
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::shared_ptr<RCommand>>::to_json(const std::shared_ptr<RCommand> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = std::dynamic_pointer_cast<NetworkCommand>(ptr)) return Json(*p);
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::unique_ptr<RCommand>>::is(const Json &json) noexcept {
        if (!json.is_object()) return false;
        if (json.template is<NetworkCommand>()) return true;
        return false;
    }

    template <typename Json>
    std::unique_ptr<RCommand> json_type_traits<Json, std::unique_ptr<RCommand>>::as(const Json &json) {
        if (!json.is_object()) return {};
        if (json.template is<NetworkCommand>()) return std::make_unique<NetworkCommand>(json.template as<NetworkCommand>());
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::unique_ptr<RCommand>>::to_json(const std::unique_ptr<RCommand> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = dynamic_cast<NetworkCommand *>(ptr.get())) return Json(*p);
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::shared_ptr<Command>>::is(const Json &json) noexcept {
        if (!json.is_object()) return false;
        if (json.template is<std::shared_ptr<RCommand>>()) return true;
        return false;
    }

    template <typename Json>
    std::shared_ptr<Command> json_type_traits<Json, std::shared_ptr<Command>>::as(const Json &json) {
        if (!json.is_object()) return {};
        if (json.template is<std::shared_ptr<RCommand>>()) return json.template as<std::shared_ptr<RCommand>>();
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::shared_ptr<Command>>::to_json(const std::shared_ptr<Command> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = std::dynamic_pointer_cast<RCommand>(ptr)) return Json(p);
        return Json::null();
    }

    template <typename Json>
    bool json_type_traits<Json, std::unique_ptr<Command>>::is(const Json &json) noexcept {
        if (!json.is_object()) return false;
        if (json.template is<RCommand>()) return true;
        return false;
    }

    template <typename Json>
    std::unique_ptr<Command> json_type_traits<Json, std::unique_ptr<Command>>::as(const Json &json) {
        if (!json.is_object()) return {};
        if (json.template is<RCommand>()) return json.template as<std::unique_ptr<RCommand>>();
        return {};
    }

    template <typename Json>
    Json json_type_traits<Json, std::unique_ptr<Command>>::to_json(const std::unique_ptr<Command> &ptr) {
        if (ptr == nullptr) return Json::null();
        if (auto p = dynamic_cast<RCommand *>(ptr.get())) return Json(std::unique_ptr<RCommand>(p));
        return Json::null();
    }
}// namespace jsoncons
