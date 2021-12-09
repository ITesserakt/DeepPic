//
// Created by tesserakt on 09.12.2021.
//

#pragma once

#include <jsoncons/json_type_traits.hpp>
#include <memory>

namespace jsoncons::polymorphic {
    template <typename T>
    struct TypeableGetter {
        constexpr static auto type() { return typeid(T).hash_code(); }

        constexpr static auto typeAccessor() { return type(); }
    };

    template <typename T>
    struct TypeableMember {
        static const unsigned long type;

        static unsigned long typeAccessor() { return type; }
    };

    template <typename T>
    const unsigned long TypeableMember<T>::type = typeid(T).hash_code();

    template <typename Base, typename Json, typename Ptr = std::shared_ptr<Base>>
    struct Definition {
        virtual bool is(const Json &) const = 0;
        virtual Ptr as(const Json &) const = 0;
        virtual Json to_json(const Ptr &) const = 0;
    };

    template <typename T, typename Json, typename Base>
    requires(!std::is_abstract_v<T>) struct TDefinitionUnique
        : jsoncons::polymorphic::Definition<Base, Json, std::unique_ptr<Base>> {
        bool is(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_IS(Command, 0, 0, T, 0)
            return false;
        }

        std::unique_ptr<Base> as(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_AS_UNIQUE_PTR(RCommand, 0, 0, T, 0)
            return {};
        }

        Json to_json(const std::unique_ptr<Base> &ptr) const override {
            JSONCONS_POLYMORPHIC_TO_JSON(RCommand, 0, 0, T, 0)
            return Json::null();
        }
    };

    template <typename T, typename Json, typename Base>
    requires(!std::is_abstract_v<T>) struct TDefinitionShared
        : jsoncons::polymorphic::Definition<Base, Json> {
        bool is(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_IS(Command, 0, 0, T, 0)
            return false;
        }

        std::shared_ptr<Base> as(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_AS(RCommand, 0, 0, T, 0)
            return {};
        }

        Json to_json(const std::shared_ptr<Base> &ptr) const override {
            JSONCONS_POLYMORPHIC_TO_JSON(RCommand, 0, 0, T, 0)
            return Json::null();
        }
    };

    template <typename T, typename Json, typename Base>
    requires std::is_abstract_v<T>
    struct AbstractDefinitionUnique : jsoncons::polymorphic::Definition<Base, Json, std::unique_ptr<Base>> {
        bool is(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_IS(Command, 0, 0, std::unique_ptr<T>, 0)
            return false;
        }

        std::unique_ptr<Base> as(const Json &ajson) const override {
            if (ajson.template is<std::unique_ptr<T>>()) return ajson.template as<std::unique_ptr<T>>();
            return {};
        }

        Json to_json(const std::unique_ptr<Base> &ptr) const override {
            if (auto p = dynamic_cast<T *>(ptr.get())) return Json(std::unique_ptr<T>(p));
            return Json::null();
        }
    };

    template <typename T, typename Json, typename Base>
    requires std::is_abstract_v<T>
    struct AbstractDefinitionShared : jsoncons::polymorphic::Definition<Base, Json> {
        bool is(const Json &ajson) const override {
            JSONCONS_POLYMORPHIC_IS(Command, 0, 0, std::shared_ptr<T>, 0)
            return false;
        }

        std::shared_ptr<Base> as(const Json &ajson) const override {
            if (ajson.template is<std::shared_ptr<T>>()) return ajson.template as<std::shared_ptr<T>>();
            return {};
        }

        Json to_json(const std::shared_ptr<Base> &ptr) const override {
            if (auto p = std::dynamic_pointer_cast<T>(ptr)) return Json(p);
            return Json::null();
        }
    };

    template <typename Base, typename Json, typename Ptr = std::shared_ptr<Base>>
    requires std::is_polymorphic_v<Base>
    class Definitions {
    private:
        friend struct json_type_traits<Json, Ptr>;

        std::vector<Definition<Base, Json, Ptr> *> defs;

    public:
        static Definitions definitions;

        Definitions() = default;

        template <typename T>
        Definitions &include() {
            if constexpr (std::same_as<std::shared_ptr<Base>, Ptr>) {
                if constexpr (!std::is_abstract_v<T>)
                    defs.emplace_back(new TDefinitionShared<T, Json, Base>{});
                else
                    defs.emplace_back(new AbstractDefinitionShared<T, Json, Base>{});
            } else {
                if constexpr (!std::is_abstract_v<T>)
                    defs.emplace_back(new TDefinitionUnique<T, Json, Base>{});
                else
                    defs.emplace_back(new AbstractDefinitionUnique<T, Json, Base>{});
            }
            return *this;
        }
    };

    template <typename T, typename Json, typename Ptr>
    requires std::is_polymorphic_v<T>
            Definitions<T, Json, Ptr> Definitions<T, Json, Ptr>::definitions = Definitions<T, Json, Ptr>();
}// namespace jsoncons::polymorphic

#define JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, typeKeyName) (type, #typeKeyName, JSONCONS_RDONLY, [](auto type) noexcept { \
    return type == T::typeAccessor();                                                                                          \
})
#define JSONCONS_POLYMORPHIC_TYPE_VAL(T) JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, type)