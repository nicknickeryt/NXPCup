#include "TCA95x5.h"

#define LOG_CHANNEL              SENSOR
#define SENSOR_LOG_CHANNEL       2
#define SENSOR_LOG_CHANNEL_LEVEL LOG_LEVEL_NOTICE

#include "logger.h"

// arduino mock
void bitWrite(int x, int n, uint8_t b) {
    if (b == 0) {
        x &= ~(1 << n);
    } else if (b == 1) {
        x |= (1 << n);
    }
}


TCA95x5::TCA95x5(NXP_I2C& Wire) : Wire(Wire) {}

/**
 * Read a specified number of bytes using the I2C bus.
 * @param output: The buffer in which to store the read values.
 * @param address: Register address to read (or starting address in burst reads)
 * @param length: Number of bytes to read.
 */
bool TCA95x5::read( uint8_t* output, tca95x5_reg_address_t address, uint8_t length) {
    bool success = true;
    Wire.beginTransmission(_device_address);
    Wire.write(address);
    Wire.endTransmission();
    // if (Wire.endTransmission() != 0) success = false;

    // else // OK, all worked, keep going
    // {
    Wire.requestFrom(_device_address, uint8_t(length));
    for (size_t i = 0; (i < length); i++) {
        uint8_t c = Wire.read();
        output[i] = c;
    }
    // }
    return success;
}

/**
 * Write a value to a register using I2C
 *
 * @param input: Byte to write to the register.
 * @param address: Address of register to write to.
 * @return: Success/error success of the write.
 */
bool TCA95x5::write(uint8_t* input, tca95x5_reg_address_t address, uint8_t length) {
    bool success = true;
    Wire.beginTransmission(_device_address);
    Wire.write(address);
    for (size_t i = 0; i < length; i++) {
        Wire.write(input[i]);
    }


    Wire.endTransmission();
    // if (Wire.endTransmission() != 0) {
    //     success = false;
    // }
    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::begin(uint8_t address) { _device_address = address & 0x27; }
void TCA95x5::begin(bool a0, bool a1, bool a2) { set_address(a0, a1, a2); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::set_address(bool a0, bool a1, bool a2) {
    uint8_t address = 0x20;
    if (a0) address |= 1;
    if (a1) address |= (1 << 1);
    if (a2) address |= (1 << 2);

    _device_address = address;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::write(tca95x5_config_t config) {
    write(config.mode);
    write(config.output);
    write(config.polarity);
}

void TCA95x5::write(tca95x5_mode_config_t config) { write((uint8_t*)&config, TCA95x5_REGISTER::CONFIG_0); }

void TCA95x5::write(tca95x5_output_config_t config) { write((uint8_t*)&config, TCA95x5_REGISTER::OUTPUT_0); }

void TCA95x5::write(tca95x5_polarity_config_t config) { write((uint8_t*)&config, TCA95x5_REGISTER::POLARITY_INV_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::read(tca95x5_config_t& config) {
    read(config.mode);
    read(config.output);
    read(config.polarity);
}

void TCA95x5::read(tca95x5_mode_config_t& config) { read((uint8_t*)&config, TCA95x5_REGISTER::CONFIG_0); }

void TCA95x5::read(tca95x5_output_config_t& config) { read((uint8_t*)&config, TCA95x5_REGISTER::OUTPUT_0); }

void TCA95x5::read(tca95x5_polarity_config_t& config) { read((uint8_t*)&config, TCA95x5_REGISTER::POLARITY_INV_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::read(tca95x5_input_status_t& status) { read((uint8_t*)&status, TCA95x5_REGISTER::INPUT_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tca95x5_config_t TCA95x5::get_config() {
    tca95x5_config_t config;
    read(config);
    return config;
}

tca95x5_mode_config_t TCA95x5::get_mode_config() {
    tca95x5_mode_config_t config;
    read(config);
    return config;
}

tca95x5_output_config_t TCA95x5::get_output_config() {
    tca95x5_output_config_t config;
    read(config);
    return config;
}

tca95x5_polarity_config_t TCA95x5::get_polarity_config() {
    tca95x5_polarity_config_t config;
    read(config);
    return config;
}

tca95x5_input_status_t TCA95x5::get_input_status() {
    tca95x5_input_status_t status;
    read(status);
    return status;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::pin_mode(tca95x5_config_t& config, size_t pin_id, tca95x5_pin_mode_t mode) { bitWrite(config.mode.raw, pin_id, mode); }

void TCA95x5::pin_write(tca95x5_config_t& config, size_t pin_id, tca95x5_pin_output_state_t state) { bitWrite(config.output.raw, pin_id, state); }
