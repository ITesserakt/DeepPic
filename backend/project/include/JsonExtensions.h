//
// Created by tesserakt on 19.11.2021.
//

#pragma once

#include "JsonPolymorphic.h"

namespace jsoncons {
    template <typename Json, typename T>
    requires std::is_polymorphic_v<T>
    struct json_type_traits<Json, std::shared_ptr<T>> {
        static bool is(const Json &json) noexcept {
            if (!json.is_object()) return false;
            for (const auto &def : polymorphic::Definitions<T, Json>::definitions.defs)
                if (!def->is(json)) return false;
            return true;
        }

        static std::shared_ptr<T> as(const Json &json) {
            if (!json.is_object()) return {};
            for (const auto &def : polymorphic::Definitions<T, Json>::definitions.defs) {
                auto result = def->as(json);
                if (result) return result;
            }
            return {};
        }

        static Json to_json(const std::shared_ptr<T> &ptr) {
            if (ptr == nullptr) return Json::null();
            for (const auto &def : polymorphic::Definitions<T, Json>::definitions.defs) {
                auto result = def->to_json(ptr);
                if (!result.is_null()) return result;
            }
            return Json::null();
        }
    };

    template <typename Json, typename T>
    requires std::is_polymorphic_v<T>
    struct json_type_traits<Json, std::unique_ptr<T>> {
        static bool is(const Json &json) noexcept {
            if (!json.is_object()) return false;
            for (const auto &def : polymorphic::Definitions<T, Json, std::unique_ptr<T>>::definitions.defs)
                if (!def->is(json)) return false;
            return true;
        }

        static std::unique_ptr<T> as(const Json &json) {
            if (!json.is_object()) return {};
            for (const auto &def : polymorphic::Definitions<T, Json, std::unique_ptr<T>>::definitions.defs) {
                auto result = def->as(json);
                if (result) return result;
            }
            return {};
        }

        static Json to_json(const std::unique_ptr<T> &ptr) {
            if (ptr == nullptr) return Json::null();
            for (const auto &def : polymorphic::Definitions<T, Json, std::unique_ptr<T>>::definitions.defs) {
                auto result = def->to_json(ptr);
                if (!result.is_null()) return result;
            }
            return Json::null();
        }
    };
}// namespace jsoncons