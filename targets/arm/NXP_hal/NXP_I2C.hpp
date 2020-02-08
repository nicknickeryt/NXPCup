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
    constexpr static uint32_t dividerTable[64] = {
        20,  22,  24,  26,   28,   30,   34,   40,   28,   32,   36,   40,   44,   48,   56,   68,
                48,  56,  64,  72,   80,   88,   104,  128,  80,   96,   112,  128,  144,  160,  192,  240,
                160, 192, 224, 256,  288,  320,  384,  480,  320,  384,  448,  512,  576,  640,  768,  960,
                640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840};

    I2C_Type* base;
    Mode mode{Mode::MASTER};
    NXP_PORT& sda;
    NXP_PORT& scl;
    uint8_t address;
    uint32_t baudrate;


public:
    NXP_I2C(I2C_Type* base, Mode mode, NXP_PORT& sda, NXP_PORT& scl, uint32_t baudrate) : base(base), mode(mode), sda(sda), scl(scl), baudrate(baudrate){}

    void init();

    void beginTransmission(uint8_t deviceAddress);

    void endTransmission();

    void write(uint8_t data);
    void write(uint8_t* data, size_t length);

    void readRegister( uint8_t deviceRegister, uint8_t* data);
    uint8_t readRegister(uint8_t deviceRegister);

    void readRequest(uint8_t deviceRegister, uint8_t* data, size_t length);

    void writeRegister(uint8_t deviceRegister, uint8_t *data, size_t length);
    void writeRegister(uint8_t deviceRegister, uint8_t data);


private:
    void wait(){
        while((base->S & I2C_S_IICIF_MASK) == 0) {
        }
        base->S |= I2C_S_IICIF_MASK; /* clear interrupt flag */
    }

    bool getAck(){ return (base->S & I2C_S_RXAK_MASK) == 0; }
    void giveAck(){base->C1 &= ~I2C_C1_TXAK_MASK; }
    void giveNack(){base->C1 |= I2C_C1_TXAK_MASK; }

    void start(){ base->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK; }
    void repeatedStart(){base->C1 |= I2C_C1_RSTA_MASK; }
    void stop(){ base->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK); }
    void enterRxModeWithAck(){ base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK); }
    void enterRxModeWithoutAck(){
        register uint8_t reg = base->C1;
        reg &= ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);
        reg |=  ((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);
        base->C1 = reg;
    }
    void enableAck(){base->C1 &= ~I2C_C1_TXAK_MASK; }
    void disableAck(){base->C1 |= I2C_C1_TXAK_MASK; }
    void writeByte(uint8_t data){base->D = data; }
    uint8_t readByte(){return base->D; }
    void setBaudrate();

};
