#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

#include "NXP_gpio.hpp"
#include "VL53L0X.h"
#include "NXP_uart.hpp"

uint_fast64_t milliseconds = 0;

uint32_t millis() {
    return milliseconds;
}
extern "C" {
    void SysTick_Handler(void) {
        milliseconds++;
    }
}

int main(void) {
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    NXP_PORT sclPort = {PORTC, 10, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_PORT sdaPort = {PORTC, 11, 2, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};

    NXP_I2C i2c = {I2C1, sdaPort, sclPort, 100000};

    NXP_PORT uartRx = {PORTA, 1, 2};
    NXP_PORT uartTx = {PORTA, 1, 2};

    NXP_Uart debugUart = {(UART_Type*)UART0, 115200, uartRx, uartTx};

    VL53L0X sensor = {i2c};

    while(1) {

    }
}
