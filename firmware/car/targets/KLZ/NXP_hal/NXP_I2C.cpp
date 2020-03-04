/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of I2C driver
 *
 */

#define LOG_CHANNEL SENSOR
#define SENSOR_LOG_CHANNEL 2
#define SENSOR_LOG_CHANNEL_LEVEL LOG_LEVEL_NOTICE

#include "NXP_I2C.hpp"
#include "logger.h"

void NXP_I2C::init() {
    sdaPort.setMux();
    sclPort.setMux();

    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudrade;
    I2C_MasterInit(base,  &masterConfig, CLOCK_GetBusClkFreq());
    I2C_MasterClearStatusFlags(base, 0xFF);
    I2C_Enable(base, true);
}

void NXP_I2C::beginTransmission(uint8_t address) {
    log_debug("begin Transmission");
    txAddress = address << 1;
    txBuffer[0] = txAddress;
    dataInTxBuffer = 1;
}

void NXP_I2C::write(uint8_t reg) {
    log_debug("write");
    txBuffer[dataInTxBuffer++] = reg;
}

void NXP_I2C::endTransmission() {
    log_debug("end Transmission");
    I2C_MasterWriteBlocking(base, txBuffer, dataInTxBuffer, kI2C_TransferDefaultFlag);
    dataInTxBuffer = 0;
}

void NXP_I2C::requestFrom(uint8_t address, uint8_t count) {
    log_debug("requestFrom");
    dataToRead = 0;
    rxAddress = address << 1 | 1u;

    I2C_MasterWriteBlocking(base, &rxAddress, 1, kI2C_TransferDefaultFlag | kI2C_TransferNoStopFlag);
    I2C_MasterReadBlocking(base, rxBuffer, count, kI2C_TransferDefaultFlag | kI2C_TransferNoStartFlag);
}

uint8_t NXP_I2C::read() {
    log_debug("read");
    return rxBuffer[dataToRead++];
}