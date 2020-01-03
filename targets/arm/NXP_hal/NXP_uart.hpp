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
    UART_Type* uart;
    uint8_t TXPin;
    uint8_t RXPin;
    uint32_t baudrate;
public:
    NXP_Uart(PORT_Type* port, UART_Type* uart, uint8_t TXPin, uint8_t RXPin, uint32_t baudrate) : port(port), uart(uart), TXPin(TXPin), RXPin(RXPin), baudrate(baudrate){}

    void init() override;

    void write(void const*) override;

    char read() override;

    void proc();
};