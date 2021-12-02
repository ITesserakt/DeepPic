//
// Created by tesserakt on 02.11.2021.
//

#pragma once

#include <concepts>

template <typename T, typename IStream, typename OStream>
concept Serializer = requires(T serializer, int data, OStream &os, IStream &is) {
    { serializer.serialize(data, os) } -> std::same_as<OStream &>;
    { serializer.template deserialize<int>(is) } -> std::same_as<int>;
};