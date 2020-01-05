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
    uint32_t baudrate;
public:
    UART_Type* uart;

private:
    void enableInterupts();

    void disableInterupts();

public:
    NXP_Uart(UART_Type* uart, uint32_t baudrate);

    void init() override;

    static void writeChar(char c);

    char read() override;

    void proc();
};