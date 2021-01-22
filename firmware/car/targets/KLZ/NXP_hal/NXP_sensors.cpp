#include "NXP_sensors.hpp"


void NXP_Sensors::init() {
    tca.begin(0, 0, 0);
    tca.write(sensorModes);
    tca.write(sensorPolarities);
}

void NXP_Sensors::selectOutput(uint8_t output) {
    TcaOutput tcaOutput = {sensorOutputs[output]};
    tca.write(tcaOutput);
}

uint8_t NXP_Sensors::findDevices() {
    uint8_t counter = 0;
    for (uint8_t i = 0; i < sensorAmount; i++) {
        VL53L0X sensor(i2c);
        // switch to next output
        selectOutput(i);
        if (sensor.init(false)) {
            Sensor newDevice = {sensor, i};
            sensorVector.push_back(newDevice);
        }
        counter++;
    }
    return counter;
}