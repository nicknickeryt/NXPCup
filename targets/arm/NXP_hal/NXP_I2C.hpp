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
    uint8_t address;


public:
    NXP_I2C(I2C_Type* base, Mode mode, NXP_PORT& sda, NXP_PORT& scl) : base(base), mode(mode), sda(sda), scl(scl){}

    void init();

    void beginTransmission(uint8_t deviceAddress);

    void endTransmission();

    void write(uint8_t index);
    void write(uint8_t* data, size_t length);
    void write(uint8_t address, uint8_t* data, size_t size);

    void requestFrom(uint8_t deviceAddress, size_t size);

    uint8_t read();

    uint8_t readRegister(uint8_t address, uint8_t deviceRegister);
    void readRegister(uint8_t address, uint8_t deviceRegister, uint8_t* data);
    void writeRegister(uint8_t address, uint8_t deviceRegister, uint8_t data);
    void writeRegister(uint8_t deviceRegister, uint8_t data);
    uint8_t readRegister(uint8_t deviceRegister);

private:
    void wait(){
        while((base->S & I2C_S_IICIF_MASK)==0) {}
        base->S |= I2C_S_IICIF_MASK; /* clear interrupt flag */
    }
    void start(){ base->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK; }
    void repeatedStart(){base->C1 |= I2C_C1_RSTA_MASK; }
    void stop(){ base->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK); }
    void enterRxMode(){ base->C1 &= ~I2C_C1_TX_MASK; base->C1 |= I2C_C1_TXAK_MASK; }
    void enableAck(){base->C1 &= ~I2C_C1_TXAK_MASK; }
    void disableAck(){base->C1 |= I2C_C1_TXAK_MASK; }
    void writeByte(uint8_t data){base->D = data; }
    uint8_t readByte(){return base->D; }



};
