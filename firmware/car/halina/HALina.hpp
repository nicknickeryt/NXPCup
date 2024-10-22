/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * HALina Main header file
 *
 * Import only this file when You want to use this library
 */
#pragma once

#include <gsl/gsl>
#include "HALina_gpio.hpp"
#include "HALina_uart.hpp"
#include "HALina_pwm.hpp"
#include "HALina_led_line.hpp"
#include "HALina_servo.hpp"
#include "HALina_motor.hpp"
#include "HALina_adc.hpp"
#include "HALina_switches.hpp"
#include "HALina_buttons.hpp"

#ifndef delay_ms
#define delay_ms(ms) for(volatile int i = 0; i < (int)CLOCK_GetFreq(kCLOCK_CoreSysClk)/10000000*4 * 9 * (int)ms; i+=1){ asm("NOP"); }
#endif