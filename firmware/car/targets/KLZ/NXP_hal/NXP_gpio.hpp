/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of gpio
 *
 */

#pragma once

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "HALina_gpio.hpp"

class NXP_GPIO : public halina::GPIO {
private:
    PORT_Type* port_base;
    GPIO_Type* base;
    uint32_t pin;
    halina::GPIO::Mode mode;
    port_interrupt_t configInterrupt;

public:
    class Interrupt {
    public:
        uint32_t mask = 0;
        void (*callbackHandler)() = nullptr;
    };

    static Interrupt portAInterrupts[32];
    static Interrupt portBInterrupts[32];
    static Interrupt portCInterrupts[32];
    static Interrupt portDInterrupts[32];
    static Interrupt portEInterrupts[32];

    void (*callbackHandler)();

public:
    NXP_GPIO(PORT_Type* port_base, GPIO_Type *base, uint32_t pin, halina::GPIO::Mode mode = halina::GPIO::Mode::OUTPUT, port_interrupt_t configInterrupt = kPORT_InterruptOrDMADisabled, void (*callbackHandler)() = nullptr):
            port_base(port_base),
            base(base),
            pin(pin),
            mode(mode),
            configInterrupt(configInterrupt),
            callbackHandler(callbackHandler) {
    }

    void init() override;

    inline void set() override {
        GPIO_SetPinsOutput(base, 1U << pin);
    }

    inline void reset() override {
        GPIO_ClearPinsOutput(base, 1U << pin);
    }

    bool get() override {
        return static_cast<bool>(GPIO_ReadPinInput(base, pin));
    }

    void toggle() override {
        GPIO_TogglePinsOutput(base, 1U << pin);
    }

    void appendInterrupt(Interrupt *portInterrupts, NXP_GPIO* gpio) {
        for (uint32_t i = 0; i < 32; i++) {
            if (portInterrupts[i].callbackHandler == nullptr) {
                portInterrupts[i].callbackHandler = gpio->callbackHandler;
                portInterrupts[i].mask = (1u << gpio->pin);
                break;
            }
        }
    }
};

class NXP_PORT : halina::PORT {
    PORT_Type* port;
    uint8_t pinNumber;
    uint32_t pinControlRegister = 0;
    uint8_t muxNumber = 0;
public:
    enum class OpenDrain : uint32_t {
#ifdef PORT_PCR_ODE
        Disable = PORT_PCR_ODE(0),
        Enable = PORT_PCR_ODE(1)
#else
        Disable = 0u << 5u,
        Enable = 1u << 5u
#endif
    };

    enum class Pull  : uint32_t {
#if defined(PORT_PCR_PE) && defined(PORT_PCR_PS)
        Disable = PORT_PCR_PE(0) | PORT_PCR_PS(0),
        PullDown = PORT_PCR_PE(1) | PORT_PCR_PS(0),
        PullUp = PORT_PCR_PE(1) | PORT_PCR_PS(1),
#else
        Disable = 0b00u,
        PullDown = 0b10,
        PullUp = 0b11
#endif
    };

    enum class SlewRate : uint32_t  {
#ifdef PORT_PCR_SRE
        Fast = PORT_PCR_SRE(0),
        Slow = PORT_PCR_SRE(1)
#else
        Fast = 0u << 2u,
        Slow = 1u << 2u
#endif
    };

    NXP_PORT(PORT_Type* port, uint8_t pinNumber, uint8_t muxNumber, Pull pull = Pull::Disable, OpenDrain openDrain = OpenDrain::Disable, SlewRate slewRate = SlewRate::Fast )
        : port(port), pinNumber(pinNumber), pinControlRegister(uint32_t(pull) | uint32_t(openDrain) | uint32_t(slewRate) | PORT_PCR_MUX(muxNumber)), muxNumber(muxNumber)  { }

    bool checkPort(){
        if(port != nullptr){
            if (port == PORTA) {
                CLOCK_EnableClock(kCLOCK_PortA);
            } else if (port == PORTB) {
                CLOCK_EnableClock(kCLOCK_PortB);
            } else if (port == PORTC) {
                CLOCK_EnableClock(kCLOCK_PortC);
            } else if (port == PORTD) {
                CLOCK_EnableClock(kCLOCK_PortD);
            } else if (port == PORTE) {
                CLOCK_EnableClock(kCLOCK_PortE);
            }
            return true;
        }
        return false;
    }

    static NXP_PORT& getEmptyPort(){
        static NXP_PORT port = {nullptr, 0, 0};
        return port;
    }

    void setMux() override {
        if(checkPort()) {
            port->PCR[pinNumber] = pinControlRegister;
        }
    }
};