#include "NXP_UART_M4.hpp"

NXP_UART* nxpUartHandlers[4];

void NXP_UART::write(uint8_t data)  {
    Chip_UART_SendByte(uart, data);
}

void NXP_UART::write(void const* data, uint16_t length) {
    __disable_irq();
    for(uint16_t i=(!txWorking); i<length; i++){
        HRingBuffer_PutChar(&txRingBuffer, *(reinterpret_cast<char*>(const_cast<void*>(data)) + i));
    }

    if (!txWorking) {
        Chip_UART_SendByte(uart, ((uint8_t*)data)[0]);
        txWorking = true;
    }
    __enable_irq();
}


static int Chip_UART_GetIndex(LPC_USART_T *pUART) {
    if(pUART == LPC_USART0) {
        return 0;
    } else if(pUART == LPC_UART1) {
        return 1;
    } else if(pUART == LPC_USART2) {
        return 2;
    } else if(pUART == LPC_USART3) {
        return 3;
    }
}

void NXP_UART::init() {
    nxpUartHandlers[Chip_UART_GetIndex(uart)] = this;

    HRingBuffer_Init(&rxRingBuffer, rxBuffer, rxBufferSize);
    HRingBuffer_Init(&txRingBuffer, txBuffer, txBufferSize);

    Chip_UART_Init(uart);
    Chip_UART_SetBaud(uart, baudrate);
    Chip_UART_TXEnable(uart);
    Chip_UART_IntEnable(uart, UART_IER_THREINT | UART_IER_RBRINT);

    if(LPC_USART0 == uart) {
        NVIC_ClearPendingIRQ(USART0_IRQn);
        NVIC_EnableIRQ(USART0_IRQn);
    } else if(LPC_UART1 == uart) {
        NVIC_ClearPendingIRQ(UART1_IRQn);
        NVIC_EnableIRQ(UART1_IRQn);
    } else if(LPC_USART2 == uart){
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_EnableIRQ(USART2_IRQn);
    } else if(LPC_USART3 == uart){
        NVIC_ClearPendingIRQ(USART3_IRQn);
        NVIC_EnableIRQ(USART3_IRQn);
    }
}

void UART_IRQ(NXP_UART* nxpUartHandler) {

    /* Handle transmit interrupt if enabled */
    if (nxpUartHandler->uart->IER & UART_IER_THREINT) {
        uint8_t ch;

        /* Fill FIFO until full or until TX ring buffer is empty (Chip_UART_ReadLineStatus(nxpUartHandler->uart) & UART_LSR_THRE) != 0 &&  */
        while (HRingBuffer_GetChar(&(nxpUartHandler->txRingBuffer), &ch)) {
            Chip_UART_SendByte(nxpUartHandler->uart, ch);
        }

        /* Turn off interrupt if the ring buffer is empty */
        if (HRingBuffer_IsEmpty(&(nxpUartHandler->rxRingBuffer))) {
            /* Shut down transmit */
            Chip_UART_IntDisable(nxpUartHandler->uart, UART_IER_THREINT);
            nxpUartHandler->txWorking = false;
        }
    }

    /* Handle receive interrupt */
    while (Chip_UART_ReadLineStatus(nxpUartHandler->uart) & UART_LSR_RDR) {
        Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, 1, 11);
        HRingBuffer_PutChar(&(nxpUartHandler->rxRingBuffer), nxpUartHandler->uart->RBR & UART_RBR_MASKBIT);
    }
}

extern "C" {
    void UART0_IRQHandler(void) {
        UART_IRQ(nxpUartHandlers[0]);
    }

    void UART1_IRQHandler(void) {
        UART_IRQ(nxpUartHandlers[1]);
    }

    void UART2_IRQHandler(void) {
        UART_IRQ(nxpUartHandlers[2]);
    }

    void UART3_IRQHandler(void) {
        UART_IRQ(nxpUartHandlers[3]);
    }
}
