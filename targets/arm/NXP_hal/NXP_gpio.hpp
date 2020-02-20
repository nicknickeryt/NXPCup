/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of gpio
 *
 */

#pragma once

#include "HALina.hpp"

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
public:
    enum class OpenDrain : uint32_t {
        Disable = 0u << 5u,
        Enable = 1u << 5u
    };

    enum class Pull  : uint32_t {
        Disable = 0b00u,
        PullDown = 0b10,
        PullUp = 0b11
    };

    enum class SlewRate : uint32_t  {
        Fast = 0u << 2u,
        Slow = 1u << 2u
    };

    NXP_PORT(PORT_Type* port, uint8_t pinNumber, uint8_t muxNumber, Pull pull = Pull::Disable, OpenDrain openDrain = OpenDrain::Disable, SlewRate slewRate = SlewRate::Fast )
        : port(port), pinNumber(pinNumber), pinControlRegister(uint32_t(pull) | uint32_t(openDrain) | uint32_t(slewRate) | (uint32_t (muxNumber << 8u) & 0xF00u)) { }

    bool checkPort(){
        return port != nullptr;
    }

    static NXP_PORT& getEmptyPort(){
        static NXP_PORT port = {nullptr, 0, 0};
        return port;
    }

    void setMux() override {
        port->PCR[pinNumber] = pinControlRegister; //PORT_PCR_MUX(muxNumber);
    }
};