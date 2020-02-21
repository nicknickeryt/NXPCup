/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#pragma once

#include <NXP_hal/VL53L0X.h>
#include "HALina.hpp"
#include "NXP_gpio.hpp"
#include "NXP_uart.hpp"
void pit_generalHandler(uint32_t*);

void pit_sendCameraData(uint8_t);

class Kitty{
private:
    // SYSTICK
    static uint_fast64_t milliseconds;

    // I2C
    NXP_PORT sclPort = {PORTC, 10, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_PORT sdaPort = {PORTC, 11, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_I2C i2c = {I2C1, sdaPort, sclPort, 100000};

    // UART
    NXP_PORT uartRx = {PORTA, 1, 2};
    NXP_PORT uartTx = {PORTA, 1, 2};

public:
    // UART
    NXP_Uart uartCommunication = {(UART_Type*)UART0, 115200, uartRx, uartTx};

    // SENOSR
    VL53L0X sensor = {i2c};

private:
    Kitty() = default;

public:
    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }

    void init();

    void proc();

    static void millisIncrease(){
        milliseconds++;
    }

    static uint32_t millis(){
        return milliseconds;
    }

    static void printCommandManager(char data){
        Kitty::kitty().uartCommunication.write(data);
    }
};