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

i2c_master_handle_t handle;
i2c_master_transfer_t transfer;

void i2c_callback(I2C_Type *base,
                  i2c_master_handle_t *handle,
                  status_t status,
                  void *userData){

}


void NXP_I2C::init() {
    if (base == I2C0){ SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; }
    else if (base == I2C1){ SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK; }
    base->F |= I2C_F_MULT(0) | I2C_F_ICR(0x12);
    base->C1 |= I2C_C1_IICEN_MASK;
    sda.setMux();
    scl.setMux();
}

void NXP_I2C::beginTransmission(uint8_t deviceAddress) {
    address = deviceAddress;
}

void NXP_I2C::endTransmission() {
    address = 0;
}

void NXP_I2C::write(uint8_t data) {
    address = (address << 1)| MWSR;
    /* send start signal */
    start();
    /* send ID with W/R bit */
    writeByte(address);
    wait();
    // write the data to the register
    writeByte(data);
    wait();
    stop();
}

void NXP_I2C::write(uint8_t address, uint8_t data) {
    address = (address << 1)| MWSR;
    /* send start signal */
    start();
    /* send ID with W/R bit */
    writeByte(address);
    wait();
    // write the data to the register
    writeByte(data);
    wait();
    stop();
}

void NXP_I2C::write(uint8_t address, uint8_t* data, size_t size) {
}

void NXP_I2C::write(uint8_t* data, size_t length) {
}

void NXP_I2C::requestFrom([[maybe_unused]]uint8_t deviceAddress, [[maybe_unused]]size_t size) {
}

uint8_t NXP_I2C::read() {
    uint8_t read = 0;
    I2C_MasterReadBlocking(base, &read, 1, kI2C_TransferDefaultFlag);
    return read;
}

void NXP_I2C::writeRegister(uint8_t address, uint8_t deviceRegister, uint8_t data){
    address = (address << 1)| MWSR;
    /* send start signal */
    start();
    /* send ID with W/R bit */
    writeByte(address);
    wait();
    // write the register address
    writeByte(deviceRegister);
    wait();
    // write the data to the register
    writeByte(data);
    wait();
    stop();
}

void NXP_I2C::writeRegister(uint8_t deviceRegister, uint8_t data){
    address = (address << 1)| MWSR;
    /* send start signal */
    start();
    /* send ID with W/R bit */
    writeByte(address);
    wait();
    // write the register address
    writeByte(deviceRegister);

    wait();
    // write the data to the register
    writeByte(data);
    wait();
    stop();
}

uint8_t NXP_I2C::readRegister(uint8_t deviceRegister){
    uint8_t data;
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    /* send start signal */
    start();
    /* send ID with Write bit */
    writeByte(addressWrite);
    wait();
    // send Register address
    writeByte(deviceRegister);
    wait();
    // send repeated start to switch to read mode
    repeatedStart();
    // re send device address with read bit
    writeByte(addressRead);
    wait();
    // set K40 in read mode
    enterRxMode();
    data = readByte();
    // send stop signal so we only read 8 bits
    stop();
    return data;
}

uint8_t NXP_I2C::readRegister(uint8_t address, uint8_t deviceRegister){
    uint8_t data;
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    /* send start signal */
    start();
    /* send ID with Write bit */
    writeByte(addressWrite);
    wait();
    // send Register address
    writeByte(deviceRegister);
    wait();
    // send repeated start to switch to read mode
    repeatedStart();
    // re send device address with read bit
    writeByte(addressRead);
    wait();
    // set K40 in read mode
    enterRxMode();
    data = readByte();
    // send stop signal so we only read 8 bits
    stop();
    return data;
}

void NXP_I2C::readRegister(uint8_t address, uint8_t deviceRegister, uint8_t* data){
    uint8_t addressWrite, addressRead;
    addressWrite = (address << 1U) | MWSR; // Write Address
    addressRead = (address << 1U) | MRSW; // Read Address
    /* send start signal */
    start();
    /* send ID with Write bit */
    writeByte(addressWrite);
    wait();
    // send Register address
    writeByte(deviceRegister);
    wait();
    // send repeated start to switch to read mode
    repeatedStart();
    // re send device address with read bit
    writeByte(addressRead);
    wait();
    // set K40 in read mode
    enterRxMode();
    *data = readByte();
    // send stop signal so we only read 8 bits
    stop();
}

extern "C"{
void I2C0_IRQHandler(){

}

void I2C1_IRQHandler(){

}
}
