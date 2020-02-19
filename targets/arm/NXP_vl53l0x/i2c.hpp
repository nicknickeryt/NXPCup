#pragma once
#include "fsl_i2c.h"


#define delayyy 1000

#define My_I2C I2C1

class i2c {
	// TX
	uint8_t txBuffer [20];
	uint8_t dataInTxBuffer = 0;
	uint8_t txAddress = 0;

	// RX
	uint8_t rxBuffer [20];
	uint8_t dataToRead = 0;
	uint8_t rxAddress = 0;

public:

	void beginTransmission(uint8_t address) {
		txAddress = address << 1;
		txBuffer[0] = txAddress;
		dataInTxBuffer = 1;
	}

	void write(uint8_t reg) {
//		if (dataInTxBuffer == 1) {
//			txBuffer[0] = txAddress;
//		}
		txBuffer[dataInTxBuffer++] = reg;
	}

	void endTransmission() {
		I2C_MasterWriteBlocking(My_I2C, txBuffer, dataInTxBuffer, kI2C_TransferDefaultFlag);
		dataInTxBuffer = 0;

		for (volatile uint32_t i = 0; i < delayyy; i++) {
			;
		}
	}

	void requestFrom(uint8_t address, uint8_t count) {
        dataToRead = 0;
		rxAddress = address << 1 | 1u;

		I2C_MasterWriteBlocking(My_I2C, &rxAddress, 1, kI2C_TransferDefaultFlag | kI2C_TransferNoStopFlag);
//		for (volatile uint32_t i = 0; i < delayyy; i++) {
//			;
//		}
		I2C_MasterReadBlocking(My_I2C, rxBuffer, count, kI2C_TransferDefaultFlag | kI2C_TransferNoStartFlag);
		for (volatile uint32_t i = 0; i < delayyy; i++) {
			;
		}
	}

	uint8_t read() {
		return rxBuffer[dataToRead++];
	}

};
