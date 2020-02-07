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

class NXP_I2C{
public:
    enum class Mode{
        MASTER,
        SLAVE
    };

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

    void requestFrom(uint8_t deviceAddress, size_t size);

    uint8_t read();

private:
    void wait(){

        while((I2C0->S & I2C_S_IICIF_MASK)==0) {}

        I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt flag */
    }

    void waitWhileBusy(){

        while((I2C0->S & I2C_S_BUSY_MASK)!=0) {}
    }

    void sendStart(){

        I2C0->C1 |= (I2C_C1_MST_MASK | I2C_C1_TX_MASK);
    }

    void sendStop(){

        I2C0->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK);
    }

    void enterTransmitMode(){

        I2C0->C1 |= I2C_C1_TX_MASK;
    }

    void enterReceiveMode(){

        I2C0->C1 &= ~I2C_C1_TX_MASK;
    }

    void enterReceiveModeWithAck(){

        I2C0->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
    }

    void enterReceiveModeWithoutAck(){

        register uint8_t reg = I2C0->C1;
        reg &= ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);
        reg |=  ((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);
        I2C0->C1 = reg;
    }

    void sendRepeatedStart(){

        I2C0->C1 |= (I2C_C1_RSTA_MASK | I2C_C1_TX_MASK);
    }

    void enableAck(){

        I2C0->C1 &= ~I2C_C1_TXAK_MASK;
    }

    void disableAck(){

        I2C0->C1 |= I2C_C1_TXAK_MASK;
    }

    void sendBlocking( const uint8_t value){

        I2C0->D = value;
    }

    uint8_t receiveDriving(){

        register uint8_t value = I2C0->D;
        wait();
        return value;
    }

     uint8_t receiveDrivingWithNack(){

        disableAck();
        return receiveDriving();
    }

    uint8_t receiveAndStop(){

        sendStop();
        return I2C0->D;
    }

};
