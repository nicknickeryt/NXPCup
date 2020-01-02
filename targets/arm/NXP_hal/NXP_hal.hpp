#ifndef PROJECT_NAME_NXP_HAL_HPP
#define PROJECT_NAME_NXP_HAL_HPP

#include "HALina.hpp"
#include "fsl_gpio.h"
#include "fsl_port.h"

class NXP_GPIO : public hal::GPIO{
    PORT_Type* port_base;
    GPIO_Type* base;
    uint32_t pin;
    hal::GPIO::Mode mode;

public:
    NXP_GPIO(PORT_Type* port_base, GPIO_Type *base, uint32_t pin, hal::GPIO::Mode mode = hal::GPIO::Mode::OUTPUT):
            port_base(port_base),
            base(base),
            pin(pin),
            mode(mode) {
    }

    void init() override {
        if (port_base == PORTA) {
            CLOCK_EnableClock(kCLOCK_PortA);
        } else if (port_base == PORTB) {
            CLOCK_EnableClock(kCLOCK_PortB);
        } else if (port_base == PORTC) {
            CLOCK_EnableClock(kCLOCK_PortC);
        } else if (port_base == PORTD) {
            CLOCK_EnableClock(kCLOCK_PortD);
        } else if (port_base == PORTE) {
            CLOCK_EnableClock(kCLOCK_PortE);
        }

        gpio_pin_config_t config;
        if (mode == hal::GPIO::Mode::OUTPUT) {
            config = {
                    kGPIO_DigitalOutput,
                    0
            };
            GPIO_PinInit(base, pin, &config);
            PORT_SetPinMux(port_base, pin, kPORT_MuxAsGpio);
        } else if (mode == hal::GPIO::Mode::INPUT) {
            config = {
                    kGPIO_DigitalInput,
                    0
            };
            GPIO_PinInit(base, pin, &config);
            PORT_SetPinMux(port_base, pin, kPORT_MuxAsGpio);
        }
    }

    inline void set() override {
        GPIO_PortSet(base, 1U << pin);
    }

    inline void reset() override {
        GPIO_PortClear(base, 1U << pin);
    }

    bool get() override {
        return static_cast<bool>(GPIO_PinRead(base, pin));
    }

    void toggle() override {
        GPIO_PortToggle(base, 1U << pin);
    }
};



#endif //PROJECT_NAME_NXP_HAL_HPP
