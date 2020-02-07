/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of I2C driver
 *
 */
#include "NXP_I2C.hpp"

void NXP_I2C::init() {
    // enable clock
    if(base == I2C0) SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
    else if(base == I2C1) SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;

    // set muxes
    sda.setMux();
    scl.setMux();

    // set freqyency
    base->F |= I2C_F_MULT(0);
    base->F |= I2C_F_ICR(0x12);

    // enable i2c module
//    base->C1 |= I2C_C1_IICEN_MASK;


    // chose master / slave mode:
    if (mode == Mode::MASTER) {
        base->C1 &= ~I2C_C1_TX_MASK;
        base->C1 |= I2C_C1_MST_MASK;
    }
    else if (mode == Mode::SLAVE) {
        base->C1 |= I2C_C1_TX_MASK;
        base->C1 &= ~I2C_C1_MST_MASK;
    }
//
//    if(base == I2C0) NVIC_EnableIRQ(I2C0_IRQn);
//    else if(base == I2C1) NVIC_EnableIRQ(I2C1_IRQn);
}

void NXP_I2C::beginTransmission(uint8_t deviceAddress){
    address = deviceAddress;
}

void NXP_I2C::endTransmission(){
    address = 0;
}

#define I2C_WRITE_ADDRESS(slaveAddress) 	((uint8_t)((slaveAddress << 1) | 0))

void NXP_I2C::write(uint8_t data){
//    waitWhileBusy();
    sendStart();
//    sendBlocking( I2C_WRITE_ADDRESS(address));
//    sendBlocking( address);
//    sendBlocking(data);
//    sendStop();
}

void NXP_I2C::requestFrom([[maybe_unused]]uint8_t deviceAddress, [[maybe_unused]]size_t size){
    enterReceiveModeWithoutAck();
}

uint8_t NXP_I2C::read(){
    waitWhileBusy();
    sendStart();
    sendBlocking(address);
    sendRepeatedStart();
    enterReceiveModeWithoutAck();
    sendStop();

    register uint8_t result = I2C0->D;
    return result;
}

extern "C"{
void I2C0_IRQHandler(){

}

void I2C1_IRQHandler(){

}
}


