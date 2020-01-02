#pragma once
#include "HALina.hpp"

class NXP_GPIO : public halina::GPIO{
    PORT_Type* port_base;
    GPIO_Type* base;
    uint32_t pin;
    halina::GPIO::Mode mode;

public:
    NXP_GPIO(PORT_Type* port_base, GPIO_Type *base, uint32_t pin, halina::GPIO::Mode mode = halina::GPIO::Mode::OUTPUT):
            port_base(port_base),
            base(base),
            pin(pin),
            mode(mode) {
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
};
