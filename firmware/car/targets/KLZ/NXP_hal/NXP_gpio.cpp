/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of gpio
 *
 */

#include "NXP_gpio.hpp"

NXP_GPIO::Interrupt NXP_GPIO::portAInterrupts[32];
NXP_GPIO::Interrupt NXP_GPIO::portDInterrupts[32];

void callFunction(PORT_Type* port_base, NXP_GPIO::Interrupt* portInterrupts) {
    uint32_t ifsr = port_base->ISFR;
    static uint32_t i;
    for(i = 0; i < 32; i++) {
        if (portInterrupts[i].mask & ifsr) {
            port_base->ISFR = portInterrupts[i].mask;
            portInterrupts[i].callbackHandler();
            return;
        }
    }
}

extern "C" {
void PORTA_IRQHandler(void) {
    callFunction(PORTA, NXP_GPIO::portAInterrupts);
}

void PORTD_IRQHandler(void) {
    callFunction(PORTD, NXP_GPIO::portDInterrupts);
}
}

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
    } else if (mode == halina::GPIO::Mode::INTERRUPT) {
        config = {
                kGPIO_DigitalInput,
                0
        };
        GPIO_PinInit(base, pin, &config);
        PORT_SetPinMux(port_base, pin, kPORT_MuxAsGpio);

        if (callbackHandler) {
            PORT_SetPinInterruptConfig(port_base, pin, configInterrupt);
        }

        if (port_base == PORTA) {
            NXP_GPIO::appendInterrupt(portAInterrupts, this);
            NVIC_ClearPendingIRQ(PORTA_IRQn);
            NVIC_EnableIRQ(PORTA_IRQn);
        } else if (port_base == PORTD) {
            NXP_GPIO::appendInterrupt(portDInterrupts, this);
            NVIC_ClearPendingIRQ(PORTD_IRQn);
            NVIC_EnableIRQ(PORTD_IRQn);
        }
    }
}