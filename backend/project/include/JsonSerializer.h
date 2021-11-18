//
// Created by tesserakt on 15.11.2021.
//

#pragma once

#include <jsoncons/json.hpp>

#include "Serializer.h"

/*
 * Basic schema for serialization:
 * {
 *      type: <type>,
 *      data: <object>
 * }
 */

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

static_assert(Serializer<JsonSerializer, std::ostream, std::istream>);