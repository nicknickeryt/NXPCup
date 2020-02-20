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
    // TX
    uint8_t txBuffer [20];
    uint8_t dataInTxBuffer = 0;
    uint8_t txAddress = 0;

    // RX
    uint8_t rxBuffer [20];
    uint8_t dataToRead = 0;
    uint8_t rxAddress = 0;
    I2C_Type *base;

    NXP_PORT& sdaPort;
    NXP_PORT& sclPort;

    uint32_t baudrade = 0;
public:
    NXP_I2C(I2C_Type *base, NXP_PORT& sdaPort, NXP_PORT& sclPort, uint32_t baudrade) : base(base), sdaPort(sdaPort), sclPort(sclPort), baudrade(baudrade) {}
    void init();
    void beginTransmission(uint8_t address);
    void write(uint8_t reg);
    void endTransmission();
    void requestFrom(uint8_t address, uint8_t count);
    uint8_t read();
};