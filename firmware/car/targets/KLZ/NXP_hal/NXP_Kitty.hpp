/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#pragma once

#include "HALina.hpp"
#include "NXP_gpio.hpp"
#include "NXP_uart.hpp"
#include "NXP_algorithm.h"
#include "NXP_sensors.hpp"

class Kitty{
private:
    // SYSTICK
    static uint_fast64_t milliseconds;

    // I2C
    NXP_PORT sclPort = {PORTC, 10, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_PORT sdaPort = {PORTC, 11, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_I2C i2c = {I2C1, sdaPort, sclPort, 100000};

    // UART
    NXP_PORT uartDebugRx = {PORTC, 3, 3};
    NXP_PORT uartDebugTx = {PORTC, 4, 3};
    NXP_PORT uartRx = {PORTD, 4, 3};
    NXP_PORT uartTx = {PORTD, 5, 3};

    // LEDS
    NXP_GPIO ledLeft = {PORTD, GPIOD, 2, NXP_GPIO::Mode::OUTPUT};
    NXP_GPIO ledLeftFront = {PORTD, GPIOD, 7, NXP_GPIO::Mode::OUTPUT};
    NXP_GPIO ledRight = {PORTE, GPIOE, 2, NXP_GPIO::Mode::OUTPUT};
    NXP_GPIO ledRightFront = {PORTE, GPIOE, 22, NXP_GPIO::Mode::OUTPUT};
    std::array<NXP_GPIO, 4> leds = {ledLeft, ledLeftFront, ledRight, ledRightFront};

    // SENSORS
    NXP_Sensors sensors = {i2c};

public:
    // UART
    NXP_Uart uartDebug = {(UART_Type*)UART1, 115200, uartDebugRx, uartDebugTx};
    NXP_Uart uartCommunication = {(UART_Type*)UART2, 115200, uartRx, uartTx};

    Algorithm algorithm = {i2c, uartCommunication, leds};


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