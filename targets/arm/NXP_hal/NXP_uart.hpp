#pragma once

#include "NXP_hal.hpp"

class NXP_Uart : public halina::UART<100, 100>{
public:
    constexpr static auto echoBufferLength = 1000;
private:
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

public:

    void init();

    void write(char);

    char read();

    void proc();
};