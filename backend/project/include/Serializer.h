//
// Created by tesserakt on 02.11.2021.
//

#pragma once

#include <boost/serialization/serialization.hpp>
#include <concepts>

template <typename T, typename IStream, typename OStream>
concept Serializer = requires(T serializer, int data, OStream &os, IStream &is) {
    { serializer.serialize(data, os) } -> std::same_as<OStream &>;
    { serializer.template deserialize<int>(is) } -> std::same_as<int>;
};

template <typename T, typename A>
concept TriviallySerializable = requires(T data, A archive, const unsigned int version) {
    { boost::serialization::serialize(archive, data, version) } -> std::same_as<void>;
};

template <typename T, typename A>
concept ObjectSerializable = requires(T data, A archive, const unsigned int version) {
    { data.serialize(archive, version) } -> std::same_as<void>;
};

template <typename T, typename A>
concept ComplexSerializable = requires(T *data, A archive, const unsigned int version) {
    { boost::serialization::load_construct_data(archive, data, version) } -> std::same_as<void>;
};

template <typename T, typename A>
concept Serializable =
        (std::default_initializable<T> && (ObjectSerializable<T, A> || TriviallySerializable<T, A>) ) ||
        (!std::default_initializable<T> && ComplexSerializable<T, A>);