#include "NXP_I2C.hpp"
#include <cstdint>

// initialize I2C
int VL53L0X_i2c_init(NXP_I2C *i2c);
int VL53L0X_write_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count, NXP_I2C *i2c);
int VL53L0X_read_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count, NXP_I2C *i2c);
int VL53L0X_write_byte(uint8_t deviceAddress, uint8_t index, uint8_t data, NXP_I2C *i2c);
int VL53L0X_write_word(uint8_t deviceAddress, uint8_t index, uint16_t data, NXP_I2C *i2c);
int VL53L0X_write_dword(uint8_t deviceAddress, uint8_t index, uint32_t data, NXP_I2C *i2c);
int VL53L0X_read_byte(uint8_t deviceAddress, uint8_t index, uint8_t *data, NXP_I2C *i2c);
int VL53L0X_read_word(uint8_t deviceAddress, uint8_t index, uint16_t *data, NXP_I2C *i2c);
int VL53L0X_read_dword(uint8_t deviceAddress, uint8_t index, uint32_t *data, NXP_I2C *i2c);
