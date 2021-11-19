//
// Created by tesserakt on 19.11.2021.
//

#pragma once

#include <jsoncons/json.hpp>

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
    requires std::is_polymorphic_v<Base>
    class Definitions {
    private:
        friend struct json_type_traits<Json, Ptr>;

        std::vector<std::function<bool(const Json &)>> is;
        std::vector<std::function<Ptr(const Json &)>> as;
        std::vector<std::function<Json(const Ptr &)>> toJson;

    public:
        static Definitions definitions;

        Definitions() = default;

        template <typename F>
        Definitions &addIsImpl(F func) {
            is.push_back(func);
            return *this;
        }

        template <typename F>
        Definitions &addAsImpl(F func) {
            as.push_back(func);
            return *this;
        }

        template <typename F>
        Definitions &addToJsonImpl(F func) {
            toJson.push_back(func);
            return *this;
        }
    };

    template <typename T, typename Json, typename Ptr>
    requires std::is_polymorphic_v<T>
            Definitions<T, Json, Ptr> Definitions<T, Json, Ptr>::definitions = Definitions<T, Json, Ptr>();
}// namespace jsoncons::polymorphic

#define JSONCONS_POLYMORPHIC_INIT_DEFINES(T) \
    struct StaticInit_##T {                  \
        StaticInit_##T();                    \
    } __dummy

#define JSONCONS_POLYMORPHIC_DEFINES_IMPLEMENT(T) T::StaticInit_##T::StaticInit_##T()

#define JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, typeKeyName) (type, #typeKeyName, JSONCONS_RDONLY, [](auto type) noexcept { \
    return type == T::typeAccessor();                                                                                          \
})
#define JSONCONS_POLYMORPHIC_TYPE_VAL(T) JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, type)