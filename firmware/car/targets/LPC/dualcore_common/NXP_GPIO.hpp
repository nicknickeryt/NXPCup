#pragma once

#include <HALina.hpp>
#include "HALina_gpio.hpp"
#include "scu_18xx_43xx.h"
#include "gpio_18xx_43xx.h"

class NXP_PORT : halina::PORT {
    uint8_t port;
    uint8_t pin;
    uint16_t function;

    NXP_PORT( uint8_t port, uint8_t pin, uint16_t function) : port(port), pin(pin), function(function) {

    }

    void setMux() override {
        Chip_SCU_PinMuxSet(port, pin, function);
    }
};

class NXP_GPIO : halina::GPIO {
    uint8_t port;
    uint8_t pin;
    halina::GPIO::Mode mode;
public:
    NXP_GPIO(uint8_t port, uint8_t pin, halina::GPIO::Mode mode = halina::GPIO::Mode::OUTPUT)
        : port(port), pin(pin), mode(mode) {

    }

    void set() override {
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, true);
    }

    void reset() override {
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, false);
    }

    bool get() override {
        return Chip_GPIO_GetPinState(LPC_GPIO_PORT, port, pin);
    }

    void toggle() override {
        Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, port, pin);
    }


    void init() override {
        if (mode == halina::GPIO::Mode::INPUT) {
            Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, port, pin);
        } else if (mode == halina::GPIO::Mode::OUTPUT) {
            Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, port, pin);
        }
    }
};
