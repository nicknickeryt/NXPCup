/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of I2C driver
 *
 */

#pragma once
#include "HALina.hpp"
#include "NXP_gpio.hpp"
#include "fsl_i2c.h"

class NXP_I2C{
public:
    enum class Mode{
        MASTER,
        SLAVE
    };

private:
    constexpr static auto MWSR = 0x00; // Master write mask
    constexpr static auto MRSW = 0x01; // Master read mask

private:
    I2C_Type* base;
    Mode mode{Mode::MASTER};
    NXP_PORT& sda;
    NXP_PORT& scl;
    uint32_t baudrate;
    i2c_master_transfer_t masterXfer;
    uint8_t address = 0;

    uint8_t txBuffer[33U];
    uint8_t rxBuffer[33U];


public:
    NXP_I2C(I2C_Type* base, Mode mode, NXP_PORT& sda, NXP_PORT& scl, uint32_t baudrate) : base(base), mode(mode), sda(sda), scl(scl), baudrate(baudrate){}

    void init();

    void beginTransmission(uint8_t deviceAddress);

    void endTransmission();

    void write(uint8_t deviceRegister, uint8_t* data, size_t length);

    void read(uint8_t deviceRegister, uint8_t* data, size_t length);

private:

};
