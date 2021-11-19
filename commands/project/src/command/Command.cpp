//
// Created by tesserakt on 19.11.2021.
//

#include "command/Command.h"

JSONCONS_POLYMORPHIC_DEFINES_IMPLEMENT(RCommand) {
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json>::definitions
            .addIsImpl([](const auto &json) { return json.template is<std::shared_ptr<RCommand>>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<std::shared_ptr<RCommand>>()) return json.template as<std::shared_ptr<RCommand>>();
                return std::shared_ptr<RCommand>(); })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = std::dynamic_pointer_cast<RCommand>(ptr)) return jsoncons::json(p);
                return jsoncons::json::null();
            });

    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json, std::unique_ptr<Command>>::definitions
            .addIsImpl([](const auto &json) { return json.template is<std::unique_ptr<RCommand>>(); })
            .addAsImpl([](const auto &json) {
                if (json.template is<std::unique_ptr<RCommand>>()) return json.template as<std::unique_ptr<RCommand>>();
                return std::unique_ptr<RCommand>(); })
            .addToJsonImpl([](const auto &ptr) {
                if (auto p = dynamic_cast<RCommand *>(ptr.get())) return jsoncons::json(std::unique_ptr<RCommand>(p));
                return jsoncons::json::null();
            });
}