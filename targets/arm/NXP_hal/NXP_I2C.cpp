/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of I2C driver
 *
 */

#define LOG_CHANNEL I2C
#define I2C_LOG_CHANNEL 2
#define I2C_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

#include "NXP_I2C.hpp"
#include "logger.h"


void NXP_I2C::init() {
    if (base == I2C0){ SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; }
    else if (base == I2C1){ SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK; }
    sda.setMux();
    scl.setMux();
    setBaudrate();
    base->C1 |= I2C_C1_IICEN_MASK;
}

void NXP_I2C::beginTransmission(uint8_t deviceAddress) {
    address = deviceAddress;
}

void NXP_I2C::endTransmission() {
    address = 0;
}

void NXP_I2C::write(uint8_t data) {
    address = (address << 1)| MWSR;
    start();
    writeByte(address);
    wait();
    if(getAck()) {
        writeByte(data);
        wait();
        getAck();
    }
    stop();
}

void NXP_I2C::write(uint8_t* data, size_t length){
    address = (address << 1)| MWSR;
    start();
    writeByte(address);
    wait();
    if(getAck()) {
        for(uint32_t i = 0; i < length; i++) {
            writeByte(data[i]);
            wait();
            getAck();
        }
    }
    stop();
}


void NXP_I2C::writeRegister( uint8_t deviceRegister, uint8_t data){
    address = (address << 1)| MWSR;
    start();
    writeByte(address);
    wait();
    if(getAck()) {
        writeByte(deviceRegister);
        wait();
        if(getAck()) {
            repeatedStart();
            writeByte(address);
            wait();
            if (getAck()) {
                writeByte(data);
                wait();
                getAck();
            }
        }
    }
    stop();
}

void NXP_I2C::writeRegister( uint8_t deviceRegister, uint8_t* data, size_t length){
    address = (address << 1)| MWSR;
    start();
    writeByte(address);
    wait();
    if(getAck()) {
        writeByte(deviceRegister);
        wait();
        if(getAck()) {
            repeatedStart();
            writeByte(address);
            wait();
            if (getAck()) {
                uint32_t i = 0;
                while(length--) {
                    writeByte(data[i]);
                    wait();
                    if(!getAck()){break; }
                    i++;
                }
            }
        }
    }
    stop();
}



uint8_t NXP_I2C::readRegister(uint8_t deviceRegister){
    uint8_t data = 0;
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    start();
    writeByte(addressWrite);
    wait();
    if(getAck()) {
        writeByte(deviceRegister);
        wait();
        if(getAck()) {
            repeatedStart();
            writeByte(addressRead);
            wait();
            if(getAck()) {
                enterRxModeWithoutAck();
                giveNack();
                data = readByte();
                wait();
            }
        }
    }
    stop();
    return data;
}


void NXP_I2C::readRegister( uint8_t deviceRegister, uint8_t* data) {
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    start();
    writeByte(addressWrite);
    wait();
    if (getAck()) {
        writeByte(deviceRegister);
        wait();
        if (getAck()) {
            repeatedStart();
            writeByte(addressRead);
            wait();
            if (getAck()) {
                enterRxModeWithoutAck();
                giveNack();
                *data = readByte();
                wait();
            }
        }
    }
    stop();
}

void NXP_I2C::readRequest(uint8_t deviceRegister, uint8_t* data, size_t length){
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    start();
    writeByte(addressWrite);
    wait();
    if (getAck()) {
        writeByte(deviceRegister);
        wait();
        if (getAck()) {
            repeatedStart();
            writeByte(addressRead);
            wait();
            if (getAck()) {
                enterRxModeWithAck();
                giveNack();
                int i = 0;
                while((length--)-1){
                    data[i] = readByte();
                    wait();
                    i++;
                    if(!getAck()) break;
                }
                enterRxModeWithoutAck();
                i++;
                data[i] = readByte();
                wait();
            }
        }
    }
    stop();
}

void NXP_I2C::setBaudrate(){
    uint32_t multiplier;
    uint32_t computedRate;
    uint32_t absError;
    uint32_t bestError = UINT32_MAX;
    uint32_t bestMult = 0u;
    uint32_t bestIcr = 0u;
    uint8_t mult;
    uint8_t i;
    /* Search for the settings with the lowest error. Mult is the MULT field of the I2C_F register,
     * and ranges from 0-2. It selects the multiplier factor for the divider. */
    for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
    {
        multiplier = 1u << mult;

        /* Scan table to find best match. */
        for (i = 0u; i < sizeof(dividerTable) / sizeof(uint16_t); ++i)
        {
            computedRate = CLOCK_GetFreq(kCLOCK_BusClk) / (multiplier * dividerTable[i]);
            absError = baudrate > computedRate ? (baudrate - computedRate) : (computedRate - baudrate);

            if (absError < bestError)
            {
                bestMult = mult;
                bestIcr = i;
                bestError = absError;

                /* If the error is 0, then we can stop searching because we won't find a better match. */
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }
    base->F = I2C_F_MULT(bestMult) | I2C_F_ICR(bestIcr);
}

extern "C"{
void I2C0_IRQHandler(){

}

void I2C1_IRQHandler(){

}
}
