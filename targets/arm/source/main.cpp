/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP Main file
 *
 */

#include <drivers/fsl_uart.h>
#include <drivers/fsl_uart_edma.h>
#include "HALina.hpp"
#include "NXP_Kitty.hpp"
#include "clock_config.h"

#define LOG_CHANNEL APP
#define APP_LOG_CHANNEL 1
#define APP_LOG_CHANNEL_LEVEL LOG_LEVEL_DEBUG

//#include "logger.h"


#include "fsl_dmamux.h"

using namespace halina;

#include "NXP_DMA.h"

///*16*/
//#define UART_OVERSAMPLE 				16
///*115200*/
///*921600*/
//#define UART_BAUDRATE 		115200
//#define UART_CLOCK			240000000
//
//#define UART_BAUD_H			((UART_CLOCK/UART_OVERSAMPLE/UART_BAUDRATE)&0x1F00)>>8
//#define UART_BAUD_L			(UART_CLOCK/UART_OVERSAMPLE/UART_BAUDRATE)&0xFF
//
//#define UART_TX_BUFFER_SIZE		10
//#define UART_RX_BUFFER_SIZE		10
//
//#define UART_TX_BUFFER_1		0
//#define UART_TX_BUFFER_2		1
//
//void UartInit(void);
//
//void UartSendChar(uint8_t data);
//
//void UartSendString(uint8_t *data);
//
//uint8_t UartGetChar();
//
//uint8_t UartRxSize();
//
//void UartDmaInit(void);
//
////extern "C"
////void DMA0_DMA16_IRQHandler(void) {
////    Kitty::kitty().ledLine.at(4).toggle();
////    DMA->INT |= (1u << 0u);
////}
//
//volatile uint8_t tx_buffer1[UART_TX_BUFFER_SIZE];
//volatile uint8_t tx_buffer2[UART_TX_BUFFER_SIZE];
//volatile uint32_t tx_element_in_buffer=0;
//volatile uint32_t tx_buffer_active=UART_TX_BUFFER_1;
//
//volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
//volatile uint32_t rx_write_buffer_ptr=0;
//volatile uint32_t rx_read_element_ptr=0;
//volatile uint32_t rx_element_in_buffer=0;
//
//
//void UartSendChar(uint8_t data){
//    if(tx_buffer_active==UART_TX_BUFFER_1){
//        tx_buffer1[tx_element_in_buffer]=data;
//        tx_element_in_buffer++;
//        if(tx_element_in_buffer>=UART_TX_BUFFER_SIZE){
//            //DMA start
//            while((DMA0->TCD[0].CSR & DMA_CSR_DONE_MASK) != DMA_CSR_DONE_MASK);
//            DMA0->TCD[0].DADDR = (uint32_t)(&(UART0->D));
//            DMA0->TCD[0].SADDR = (uint32_t)tx_buffer1;//defines source data address
//            DMA0->TCD[0].CITER_ELINKNO = UART_TX_BUFFER_SIZE;//CITER=1
//            DMA0->TCD[0].BITER_ELINKNO = UART_TX_BUFFER_SIZE;//BITER=1
//            DMA0->SERQ = DMA_SERQ_SERQ(0);
//            //
//            tx_element_in_buffer=0;
//            tx_buffer_active=UART_TX_BUFFER_2;
//        }
//    }else{
//        tx_buffer2[tx_element_in_buffer]=data;
//        tx_element_in_buffer++;
//        if(tx_element_in_buffer>=UART_TX_BUFFER_SIZE){
//            //DMA start
//            while((DMA0->TCD[0].CSR & DMA_CSR_DONE_MASK) != DMA_CSR_DONE_MASK);
//            DMA0->TCD[0].DADDR = (uint32_t)(&(UART0->D));
//            DMA0->TCD[0].SADDR = (uint32_t)tx_buffer2;//defines source data address
//            DMA0->TCD[0].CITER_ELINKNO = UART_TX_BUFFER_SIZE;//CITER=1
//            DMA0->TCD[0].BITER_ELINKNO = UART_TX_BUFFER_SIZE;//BITER=1
//            DMA0->SERQ = DMA_SERQ_SERQ(0);
//            //
//            tx_element_in_buffer=0;
//            tx_buffer_active=UART_TX_BUFFER_1;
//        }
//    }
//}
//void UartDmaInit(void){
//    //DMA MUX initialization
////    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK; //enables DMA MUX clock gate
////    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;//enables DMA clock gate, default value is enable
//////
////    DMAMUX0->CHCFG[0] = 0; //clears register for changing source and trigger
////    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(0x03); //source number is 3, UART0 transmit
//
//    DMA0->ERQ |= 0x01;//enables p request
////
//    DMA0->TCD[0].SADDR = (uint32_t)tx_buffer1;//defines source data address
//    DMA0->TCD[0].SOFF = 1;//Source address signed offset
//    DMA0->TCD[0].DADDR = (uint32_t)(&UART0->D);//defines destination data address
//    DMA0->TCD[0].CITER_ELINKNO = 0x01;//CITER=1
//    DMA0->TCD[0].BITER_ELINKNO = 0x01;//BITER=1
//    DMA0->TCD[0].NBYTES_MLNO = 1;//byte number
//    DMA0->TCD[0].DOFF = 0;//destination address signed offset
//    DMA0->TCD[0].ATTR =  0;//8 bit transfer size, register default value is undefined
//    DMA0->TCD[0].SLAST = 0;//restores the source address to the initial value
//    DMA0->TCD[0].DLAST_SGA = 0;//restores the destination address to the initial value
////////
////    DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK | DMA_CSR_DREQ_MASK | DMA_CSR_START_MASK;//The end-of-major loop interrupt is enabled
////
////    DMA->INT |= (1u << 0u);
//////
//    NVIC_ClearPendingIRQ(DMA0_DMA16_IRQn);
//    NVIC_EnableIRQ(DMA0_DMA16_IRQn);
//}




int main(){
    BOARD_BootClockRUN();
    Kitty& kitty = Kitty::kitty();
    kitty.init();

    NXP_DMA uartDMA = NXP_DMA(0, kDmaRequestMux0UART0Tx);

    NXP_PORT uart0RXmux = {PORTA, 14U, 0x03};
    NXP_PORT uart0TXmux = {PORTA, 15U, 0x03};
    NXP_Uart uartCommunication = {UART0, 115200, uart0RXmux, uart0TXmux, uartDMA};
    uartCommunication.init();
    uartCommunication.DMAinit();

//    uartDMA.init();

//    NVIC_ClearPendingIRQ(DMA0_DMA16_IRQn);
//    NVIC_EnableIRQ(DMA0_DMA16_IRQn);
//    uartCommunication.DMAinit();

//    UartInit();
//    UartDmaInit();


//    for (uint8_t i = 0; i < 10; i++) {
//        UartSendChar('d');
//    }
//
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//    UartSendChar('f');
//
//    for (volatile uint32_t i = 0; i < 2000000; i++) {
////        UartSendChar('d');
//    }
//
//    for (uint8_t i = 0; i < 10; i++) {
//        UartSendChar('d');
//    }

//    kitty.camera.start();
    while (true){
//        DMA0->SERQ = DMA_SERQ_SERQ(0);
        kitty.proc();
//        UartSendChar('d');
//        kitty.servo.set(0.4f);
    }
}



