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
#include "MKV58F24.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "HALina_gpio.hpp"
#include "HALina_uart.hpp"
#include "HALina_pwm.hpp"
#include "HALina_led_line.hpp"
#include "HALina_servo.hpp"
#include "HALina_motor.hpp"
