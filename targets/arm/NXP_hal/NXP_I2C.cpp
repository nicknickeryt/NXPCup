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
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudrate;
    I2C_MasterInit(base, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
    memset(&masterXfer, 0, sizeof(masterXfer));
}

void NXP_I2C::beginTransmission(uint8_t deviceAddress) {
    masterXfer.slaveAddress   = deviceAddress;
}

void NXP_I2C::endTransmission() {
    masterXfer.slaveAddress   = 0;
    masterXfer.direction      = i2c_direction_t(0);
    masterXfer.subaddress     = 0;
    masterXfer.subaddressSize = 0;
}

void NXP_I2C::write(uint8_t deviceRegister, uint8_t* data, size_t length){
    masterXfer.subaddress = (uint32_t)deviceRegister;
    masterXfer.subaddressSize = 1;
    masterXfer.data = data;
    masterXfer.dataSize = length;
    masterXfer.direction = kI2C_Write;
    I2C_MasterTransferBlocking(base, &masterXfer);
}

void NXP_I2C::read(uint8_t deviceRegister, uint8_t* data, size_t length){
    masterXfer.subaddress = (uint32_t)deviceRegister;
    masterXfer.subaddressSize = 1;
    masterXfer.dataSize = length;
    masterXfer.direction = kI2C_Read;
    I2C_MasterTransferBlocking(base, &masterXfer);
    memcpy(data, masterXfer.data, length);
}

extern "C"{
void I2C0_IRQHandler(){

}

void I2C1_IRQHandler(){

}
}
