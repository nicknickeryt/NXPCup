//
// Created by Witek on 05.01.2020.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HALina_display.hpp"
#include <iostream>

class DisplayMOCK : public HALina_Display {
public:
    MOCK_METHOD(void, update, ());
    MOCK_METHOD(void, init, ());
};

bool stringCompare(const char * data1, const char * data2) {
    char temp[4] = {0};
    memcpy(temp, data2, 4);
    for(uint8_t i = 0; i < 4; i ++) {
        if (temp[i] >= 0 && temp[i] <= 9) {
            temp[i] += 48;
        } else if (temp[i] == uint8_t(HALina_Display::SPECIAL_CHARACTERS::SPACE)) {
            temp[i] = ' ';
        }  else if (temp[i] == uint8_t(HALina_Display::SPECIAL_CHARACTERS::DASH)) {
            temp[i] = '-';
        }
    }
    std::cout << data1 << " " << temp << std::endl;
    return !static_cast<bool>(strcmp(data1, temp));
}

TEST(DISPLAY, char_data) {
    DisplayMOCK display;

    display.print("1234");

    EXPECT_EQ(true, stringCompare("1234", (const char*) display.getBuffer()));
}

TEST(DISPLAY, uint_data) {
    DisplayMOCK display;

    display.print(1234);
    EXPECT_EQ(true, stringCompare("1234", (const char*) display.getBuffer()));

    display.print(4321);
    EXPECT_EQ(false, stringCompare("1234", (const char*) display.getBuffer()));

    display.print(10234u);
    EXPECT_EQ(true, stringCompare("----", (const char*) display.getBuffer()));
}

TEST(DISPLAY, int_data) {
    DisplayMOCK display;

    display.print(-234);
    EXPECT_EQ(true, stringCompare("-234", (const char*) display.getBuffer()));

    display.print(234);
    EXPECT_EQ(true, stringCompare("0234", (const char*) display.getBuffer()));

    display.print(-9999);
    EXPECT_EQ(true, stringCompare("----", (const char*) display.getBuffer()));

    display.print(-11999);
    EXPECT_EQ(true, stringCompare("----", (const char*) display.getBuffer()));
}

TEST(DISPLAY, float_data) {
    DisplayMOCK display;

    display.print(-1.0, 1);
    EXPECT_EQ(true, stringCompare("-010", (const char*) display.getBuffer()));

    display.print(-1.23, 2);
    EXPECT_EQ(true, stringCompare("-123", (const char*) display.getBuffer()));
}