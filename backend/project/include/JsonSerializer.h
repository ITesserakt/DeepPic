//
// Created by tesserakt on 15.11.2021.
//

#pragma once

#include "utility"
#include <jsoncons/json.hpp>

#include "Serializer.h"

struct JsonSerializer {
    template <typename T, typename O>
    O &serialize(T &data, O &os) {
        jsoncons::encode_json(data, os);
        return os;
    }

    template <typename T, typename I>
    T deserialize(I &is) {
        return jsoncons::decode_json<T>(is);
    }
};

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
}// namespace jsoncons::polymorphic

#define JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, typeKeyName) (type, #typeKeyName, JSONCONS_RDONLY, [](auto type) noexcept { \
    return type == T::typeAccessor();                                                                                          \
})
#define JSONCONS_POLYMORPHIC_TYPE_VAL(T) JSONCONS_POLYMORPHIC_TYPE_VAL_WITH_NAME(T, type)

static_assert(Serializer<JsonSerializer, std::ostream, std::istream>);