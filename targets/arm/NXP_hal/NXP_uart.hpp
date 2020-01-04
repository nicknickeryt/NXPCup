/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of uart
 *
 */

#pragma once

#include "HALina.hpp"

class NXP_Uart : public halina::UART{
private:
    PORT_Type* port;
    uint8_t TXPin;
    uint8_t RXPin;
    uint32_t baudrate;
public:
    UART_Type* uart;

    NXP_Uart(PORT_Type* port, UART_Type* uart, uint8_t TXPin, uint8_t RXPin, uint32_t baudrate);

    void init() override;

    void write(void const*) override;

    static void writeChar(const char c);

    char read() override;

    void proc();
};