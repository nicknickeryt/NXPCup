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
#include "NXP_display.hpp"
#include "NXP_servo.hpp"
#include "NXP_motor.hpp"
#include "NXP_PIT.hpp"

class Kitty{
private:
    NXP_GPIO LED0 = NXP_GPIO(PORTA, GPIOA, 16U);
    NXP_GPIO LED1 = NXP_GPIO(PORTA, GPIOA, 17U);
    NXP_GPIO LED2 = NXP_GPIO(PORTA, GPIOA, 24U);
    NXP_GPIO LED3 = NXP_GPIO(PORTA, GPIOA, 25U);
    NXP_GPIO LED4 = NXP_GPIO(PORTA, GPIOA, 26U);
    NXP_GPIO LED5 = NXP_GPIO(PORTA, GPIOA, 27U);
    NXP_GPIO LED6 = NXP_GPIO(PORTA, GPIOA, 28U);
    NXP_GPIO LED7 = NXP_GPIO(PORTA, GPIOA, 29U);
    NXP_GPIO intr = {PORTA, GPIOA, 13, halina::GPIO::Mode::INTERRUPT, kPORT_InterruptRisingEdge, nullptr};
    NXP_GPIO intl = {PORTB, GPIOB, 19, halina::GPIO::Mode::INTERRUPT, kPORT_InterruptRisingEdge, nullptr};

    NXP_GPIO motorEnablePin = NXP_GPIO(PORTE, GPIOE, 4U);

    NXP_PORT servoPort = {PORTA, 7, 0x03};
    NXP_PORT motorLeftPortMLB = {PORTE, 5, 0x06};
    NXP_PORT motorLeftPortMLF = {PORTE, 6, 0x06};
    NXP_PORT motorRightPortMLB = {PORTE, 7, 0x06};
    NXP_PORT motorRightPortMLF = {PORTE, 8, 0x06};

    NXP_PWM servoPwm = {*this, FTM0, servoPort, NXP_PORT::getEmptyPort(), 4, 0, BOARD_BOOTCLOCKRUN_CORE_CLOCK/64/50};

    NXP_PWM motorLeftPwm = {*this, FTM3, motorLeftPortMLB, motorLeftPortMLF, 0, 1, BOARD_BOOTCLOCKRUN_CORE_CLOCK/1/10000};
    NXP_PWM motorRightPwm = {*this, FTM3, motorRightPortMLB, motorRightPortMLF, 2, 3, BOARD_BOOTCLOCKRUN_CORE_CLOCK/1/10000};

    NXP_Motor motorLeft = {*this, motorLeftPwm, motorEnablePin, -5000, 5000};
    NXP_Motor motorRight = {*this, motorRightPwm, motorEnablePin, -5000, 5000};

    NXP_PIT pit0 = {NXP_PIT::CHANNEL::_0, 2, nullptr};
    NXP_PIT pit1 = {NXP_PIT::CHANNEL::_1, 2, nullptr};

public:
    NXP_Uart uartDebug = {PORTA, UART0, 14U, 15U, 115200};
    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};
    NXP_Display display;
    NXP_Servo servo = {*this, servoPwm, 1400, 4400};
    NXP_Motors motors = {motorLeft, motorRight};
private:
    Kitty() = default;

    void FTM_Init();

    void magicDiodComposition();

public:
    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }

    void init();

    void proc();

};