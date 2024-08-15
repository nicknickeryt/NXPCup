/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * MKV HALina implementation of gpio
 *
 */

#include "MKV_gpio.hpp"

MKV_GPIO::Interrupt MKV_GPIO::portAInterrupts[32];
MKV_GPIO::Interrupt MKV_GPIO::portBInterrupts[32];
MKV_GPIO::Interrupt MKV_GPIO::portCInterrupts[32];
MKV_GPIO::Interrupt MKV_GPIO::portDInterrupts[32];
MKV_GPIO::Interrupt MKV_GPIO::portEInterrupts[32];

void callFunction(PORT_Type* port_base, MKV_GPIO::Interrupt* portInterrupts) {
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
    callFunction(PORTA, MKV_GPIO::portAInterrupts);
}

void PORTB_IRQHandler(void) {
    callFunction(PORTB, MKV_GPIO::portBInterrupts);
}

void PORTC_IRQHandler(void) {
    callFunction(PORTC, MKV_GPIO::portCInterrupts);
}

void PORTD_IRQHandler(void) {
    callFunction(PORTD, MKV_GPIO::portDInterrupts);
}

void PORTE_IRQHandler(void) {
    callFunction(PORTE, MKV_GPIO::portEInterrupts);
}
}

void MKV_GPIO::init() {
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
            MKV_GPIO::appendInterrupt(portAInterrupts, this);
            NVIC_ClearPendingIRQ(PORTA_IRQn);
            NVIC_EnableIRQ(PORTA_IRQn);
        } else if (port_base == PORTB) {
            MKV_GPIO::appendInterrupt(portBInterrupts, this);
            NVIC_ClearPendingIRQ(PORTB_IRQn);
            NVIC_EnableIRQ(PORTB_IRQn);
        } else if (port_base == PORTC) {
            MKV_GPIO::appendInterrupt(portCInterrupts, this);
            NVIC_ClearPendingIRQ(PORTC_IRQn);
            NVIC_EnableIRQ(PORTC_IRQn);
        } else if (port_base == PORTD) {
            MKV_GPIO::appendInterrupt(portDInterrupts, this);
            NVIC_ClearPendingIRQ(PORTD_IRQn);
            NVIC_EnableIRQ(PORTD_IRQn);
        } else if (port_base == PORTE) {
            MKV_GPIO::appendInterrupt(portEInterrupts, this);
            NVIC_ClearPendingIRQ(PORTE_IRQn);
            NVIC_EnableIRQ(PORTE_IRQn);
        }
    }
}