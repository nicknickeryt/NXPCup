#pragma once

#include "HALina_ring_buffer_MB.hpp"
#include "HALina_uart.hpp"
#include "chip.h"


class NXP_UART : public halina::UART {
    uint32_t baudrate;


    constexpr static auto txBufferSize = 2048;
    constexpr static auto rxBufferSize = 1024;

    enum class InterruptType : uint8_t {
        TX_EMPTY = UART_LSR_THRE,
        TX_COMPLETE = UART_LSR_TEMT,
        RX_FULL = UART_LSR_RDR
    };

public:
    NXP_UART(LPC_USART_T * uart, uint32_t baudrate) : uart(uart), baudrate(baudrate) {

    }

    void init() override;

    void write(void const* data, uint16_t length) override;
    void write(uint8_t data) override;
    void write_u(uint32_t data) {
        char buffer[10];
        itoa (data,buffer,10);
        write(buffer, strlen(buffer));
    }

    bool txWorking = false;

    uint8_t read() override {

    }


    void setRedirectHandler( void (*redirectHandler)(uint8_t)) {
        this->redirectHandler = redirectHandler;
    }


    LPC_USART_T *uart;
    uint8_t rxBuffer[rxBufferSize] = {0};
    uint8_t txBuffer[txBufferSize] = {0};

    RingBuffer rxRingBuffer;
    RingBuffer txRingBuffer;

    void (*redirectHandler)(uint8_t){};
};
