//
// Created by eremey on 18.11.2021.
//
#include <gtest/gtest.h>
#include "MainWindow.h"
#include <string.h>

#include <QApplication>

TEST(INVALID_SIZE, test1) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = -1;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid size") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidRed1) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = -1;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid red color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidRed2) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 300;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid red color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidGreen1) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = -1;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid green color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidGreen2) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 300;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid green color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidBlue1) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 40;
    curve.color_blue = -1;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid blue color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_COLOR, invalidBlue2) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 40;
    curve.color_blue = 300;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid blue color") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_LENGTH, invalidLength1) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 40;
    curve.color_blue = 30;

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid curve size") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(INVALID_LENGTH, invalidLength2) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 40;
    curve.color_blue = 30;
    curve.coords.emplace_back(QPointF(1,1));

    bool isSuccessTest = false;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        if (strcmp(ex.what(), "Invalid curve size") == 0) {
            isSuccessTest = true;
        }
    }
    EXPECT_EQ(isSuccessTest, true);
}

TEST(VALID_CURVE, validCurve) {
    MainWindow mainWindow;
    Curve curve;
    curve.brush_size = 10;
    curve.color_red = 20;
    curve.color_green = 40;
    curve.color_blue = 30;
    curve.coords.emplace_back(QPointF(1,1));
    curve.coords.emplace_back(QPointF(2,2));

    bool isSuccessTest = true;
    try {
        mainWindow.executeBrush(curve);
    }

    catch (std::invalid_argument const& ex) {
        isSuccessTest = false;
    }
    EXPECT_EQ(isSuccessTest, true);
}

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}