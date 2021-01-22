#include "NXP_I2C.hpp"
#include "TCA95x5.h"
#include "VL53L0X.h"

#include <vector>

class NXP_Sensors {
  private:
    using TcaOutput = tca95x5_output_config_t;

    constexpr static auto                      sensorAmount                = 6;
    constexpr static tca95x5_mode_config_t     sensorModes                 = {0b1001010100000101};
    constexpr static tca95x5_polarity_config_t sensorPolarities            = {0x0000};
    constexpr static uint16_t                  sensorOutputs[sensorAmount] = {0x0800, 0x2000, 0x4000, 0x0200, 0x0008, 0x0002};

    struct Sensor {
        VL53L0X device;
        uint8_t address;
    };

    NXP_I2C& i2c;

    TCA95x5             tca = {i2c};
    std::vector<Sensor> sensorVector;


  public:
    NXP_Sensors(NXP_I2C& i2c) : i2c(i2c) {}

    void init();

    void    selectOutput(uint8_t output);
    uint8_t findDevices();
};