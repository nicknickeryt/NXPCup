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

    NXP_I2C(I2C_Type *base, NXP_PORT& sdaPort, NXP_PORT& sclPort, uint32_t baudrade) : base(base), sdaPort(sdaPort), sclPort(sclPort), baudrade(baudrade) {

    }

    void init() {
        sdaPort.setMux();
        sclPort.setMux();

        i2c_master_config_t masterConfig;
        I2C_MasterGetDefaultConfig(&masterConfig);
        masterConfig.baudRate_Bps = baudrade;
        I2C_MasterInit(base,  &masterConfig, CLOCK_GetBusClkFreq());
        I2C_MasterClearStatusFlags(base, 0xFF);
        I2C_Enable(base, true);
    }

        void beginTransmission(uint8_t address) {
            txAddress = address << 1;
            txBuffer[0] = txAddress;
            dataInTxBuffer = 1;
        }

        void write(uint8_t reg) {
            txBuffer[dataInTxBuffer++] = reg;
        }

        void endTransmission() {
            I2C_MasterWriteBlocking(base, txBuffer, dataInTxBuffer, kI2C_TransferDefaultFlag);
            dataInTxBuffer = 0;
        }

        void requestFrom(uint8_t address, uint8_t count) {
            dataToRead = 0;
            rxAddress = address << 1 | 1u;

            I2C_MasterWriteBlocking(base, &rxAddress, 1, kI2C_TransferDefaultFlag | kI2C_TransferNoStopFlag);
            I2C_MasterReadBlocking(base, rxBuffer, count, kI2C_TransferDefaultFlag | kI2C_TransferNoStartFlag);
        }

        uint8_t read() {
            return rxBuffer[dataToRead++];
        }

    };