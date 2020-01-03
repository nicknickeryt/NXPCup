/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina configuration file
 *
 */

#pragma once

/////////////////////// UART ///////////////////////
#define UART					UART0
#define UART_PORT				PORTA
#define UART_RX_PIN				14U
#define UART_TX_PIN				15U
#define UART_BAUDRATE			115200

#define DMA						DMA0

/////////////////////// LOG ///////////////////////
#define ENABLE_LOG				1


/////////////////////// DISPLAY ///////////////////////
#define DISPLAY_GPIO			GPIOC
#define DISPLAY_PORT			PORTC

// set refresh rate when using in systic
#define REFRESH_RATE_HZ			10
#define SYSTICK_RATE_HZ			1000

#define DISPLAY_A1				((uint32_t)(1<<0))
#define DISPLAY_A2				((uint32_t)(1<<1))
#define DISPLAY_A3				((uint32_t)(1<<2))
#define DISPLAY_A4				((uint32_t)(1<<5))

#define DISPLAY_A				((uint32_t)(1<<8))
#define DISPLAY_B				((uint32_t)(1<<9))
#define DISPLAY_C				((uint32_t)(1<<11))
#define DISPLAY_D				((uint32_t)(1<<14))
#define DISPLAY_E				((uint32_t)(1<<13))
#define DISPLAY_F				((uint32_t)(1<<10))
#define DISPLAY_G				((uint32_t)(1<<15))
#define DISPLAY_DP				((uint32_t)(1<<12))

/////////////////////// MOTORS ///////////////////////
#define MOTOR_CLOCK_PRESCALER 		1
#define MOTOR_CLOCK_INPUT			DEFAULT_SYSTEM_CLOCK
#define MOTOR_CLOCK_OUTPUT			10000
#define MOTOR_CLOCK_MOD				MOTOR_CLOCK_INPUT/MOTOR_CLOCK_PRESCALER/MOTOR_CLOCK_OUTPUT
