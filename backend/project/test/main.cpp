//
// Created by potry on 11/18/2021.
//

#include <gtest/gtest.h>

#include "JsonSerializer.h"

using SerializerType = JsonSerializer;

struct Point {
    int x, y;

    void serialize(auto &ar, const unsigned int) {
        ar &x &y;
    }

    bool operator==(const Point &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Point &rhs) const {
        return !(rhs == *this);
    }
};

JSONCONS_ALL_MEMBER_TRAITS(Point, x, y)

TEST(serializing, serialize_simple_types) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = 1;

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    EXPECT_EQ(serializer.deserialize<decltype(value)>(stream), value);
}

TEST(serializing, serialize_complex_types) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = Point{1, 2};

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    auto result = serializer.deserialize<Point>(stream);
    EXPECT_EQ(result, value);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}