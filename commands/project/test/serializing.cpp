//
// Created by potry on 11/18/2021.
//

#include <gtest/gtest.h>
#include <memory>

#include "JsonSerializer.h"
#include "JsonconsDefinitions.h"

using SerializerType = JsonSerializer;

TEST(serializing, serialize_command) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = HelloCommand(5);

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    EXPECT_EQ(serializer.deserialize<HelloCommand>(stream), value);
}

TEST(serializing, serialize_polymorphic_type) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = HelloCommand(42);

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    auto cmd = serializer.deserialize<std::shared_ptr<Command>>(stream);
    EXPECT_NE(cmd, nullptr);
    EXPECT_EQ(*std::dynamic_pointer_cast<HelloCommand>(cmd), value);
}

TEST(serializing, serialize_network_command) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = NetworkCommand(0, new HelloCommand(42));

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    auto cmd = serializer.deserialize<std::shared_ptr<Command>>(stream);
    EXPECT_NE(cmd, nullptr);
    EXPECT_EQ(std::dynamic_pointer_cast<NetworkCommand>(cmd)->inner<HelloCommand>(), value.inner<HelloCommand>());
}