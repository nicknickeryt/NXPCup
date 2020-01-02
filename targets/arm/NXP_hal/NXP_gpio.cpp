#include "NXP_hal.hpp"

void NXP_GPIO::init() {
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
    if (mode == halina::GPIO::Mode::OUTPUT) {
        config = {
                kGPIO_DigitalOutput,
                0
        };
        GPIO_PinInit(base, pin, &config);
        PORT_SetPinMux(port_base, pin, kPORT_MuxAsGpio);
    } else if (mode == halina::GPIO::Mode::INPUT) {
        config = {
                kGPIO_DigitalInput,
                0
        };
        GPIO_PinInit(base, pin, &config);
        PORT_SetPinMux(port_base, pin, kPORT_MuxAsGpio);
    }
}