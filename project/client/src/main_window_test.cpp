//
// Created by eremey on 18.11.2021.
//
#include <gtest/gtest.h>
#include "MainWindow.h"

TEST(GET_TIME, get_time) {
    EXPECT_EQ(-1, -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}