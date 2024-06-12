/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina 7 segment display driver
 *
 */

#pragma once

#include <gsl/gsl>
#include <array>
#include <cstring>

class HALina_Display {
protected:
    uint8_t displayBuffer[4] = {0, 0, 0, 0};
    std::array<bool, 4> dot = {false};

public:
    virtual void update() = 0;
    virtual void init() = 0;

    enum class SPECIAL_CHARACTERS : uint8_t {
        DASH = 10,
        SPACE = 11
    };

    void print(const char* data) {
        memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE), 4);
        dot = {false};
        for(auto i = 0; i < 4; i++) {
            uint8_t digit = data[i];
            if (digit >= '0' && digit <= '9') {
                displayBuffer[i] = digit - 48;
            } else if (digit == '-') {
                displayBuffer[i] = static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH);
            } else {
                displayBuffer[i] = static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE);
            }
        }
    }

    uint8_t* getBuffer() {
        return &displayBuffer[0];
    }

    void print(uint32_t data) {
        memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE), 4);
        dot = {false};
        if(data < 10000) {
            displayBuffer[0] = (data / 1000);
            data -= ((data / 1000) * 1000);
            displayBuffer[1] = (data / 100);
            data -= ((data / 100) * 100);
            displayBuffer[2] = data / 10;
            data -= ((data / 10) * 10);
            displayBuffer[3] = data;
        } else {
            memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH), 4);
        }
    }

    void print(int data) {
        memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE), 4);
        dot = {false};
        if (data < -999 || data > 9999) {
            memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH), 4);
            return;
        }

        if (data < 0) {
            displayBuffer[0] = static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH);
            data = -data;
        } else {
            displayBuffer[0] = (data / 1000) ;
            data -= ((data / 1000) * 1000);
        }

        displayBuffer[1] = (data / 100);
        data -= ((data / 100) * 100);
        displayBuffer[2] = data / 10;
        data -= ((data / 10) * 10);
        displayBuffer[3] = data;
    }


    void print(float data, uint8_t precision) {
        dot = {false};
        memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE), 4);
        uint32_t uintData = 0;

        if (data < 0.0f) {
            displayBuffer[0] = static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH);
            data = -data;
        } else {
            displayBuffer[0] = static_cast<uint8_t >(SPECIAL_CHARACTERS::SPACE);
        }

        if (precision == 1) {
            if (data > 99.9f) {
                memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH), 4);
                return;
            }
            uintData = static_cast<uint32_t >(data * 10.0f);
            dot[2] = true;
        } else if (precision == 2) {
            if (data > 9.99f) {
                memset(displayBuffer, static_cast<uint8_t >(SPECIAL_CHARACTERS::DASH), 4);
                return;
            }

            uintData = static_cast<uint32_t >(data * 100.0f);
            dot[1] = true;
        }
        displayBuffer[1] = (uintData / 100);
        uintData -= ((uintData / 100) * 100);
        displayBuffer[2] = uintData / 10;
        uintData -= ((uintData / 10) * 10);
        displayBuffer[3] = uintData;
    }
};

