#pragma once

#include "NXP_hal.hpp"

class NXP_Uart : public halina::UART<100, 100>{
public:
    constexpr static auto bufferLength = 1000;

public:
    void init() override;

    void write(void const*) override;

    char read() override;

    void proc();
};