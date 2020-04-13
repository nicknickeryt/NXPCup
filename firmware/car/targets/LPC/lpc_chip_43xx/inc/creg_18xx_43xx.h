/*
 * @brief LPC18XX/43XX CREG control functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __CREG_18XX_43XX_H_
#define __CREG_18XX_43XX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* CREG0  at 0x40043004*/
/* CREG_CREG0_EN1KHZ Enable 1 kHz output.
 * 	 0x00 1 kHz output disabled.
 * 	 0x01 1 kHz output enabled.
 */
#define CREG_CREG0_EN1KHZ_SHIFT	(0U)
#define CREG_CREG0_EN1KHZ_MASK 	(0x01U << CREG_CREG0_EN1KHZ_SHIFT)
#define CREG_CREG0_EN1KHZ(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_EN1KHZ_SHIFT)) & CREG_CREG0_EN1KHZ_MASK)

/* CREG_CREG0_EN32KHZ Enable 32 kHz output
 * 	 0x00 32 kHz output disabled.
 * 	 0x01 32 kHz output enabled.
 */
#define CREG_CREG0_EN32KHZ_SHIFT	(1U)
#define CREG_CREG0_EN32KHZ_MASK 	(0x01U << CREG_CREG0_EN32KHZ_SHIFT)
#define CREG_CREG0_EN32KHZ(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_EN32KHZ_SHIFT)) & CREG_CREG0_EN32KHZ_MASK)

/* CREG_CREG0_RESET32KHZ 32 kHz oscillator reset
 * 	 0x00 Clear reset.
 * 	 0x01 Reset active.
 */
#define CREG_CREG0_RESET32KHZ_SHIFT	(2U)
#define CREG_CREG0_RESET32KHZ_MASK 	(0x01U << CREG_CREG0_RESET32KHZ_SHIFT)
#define CREG_CREG0_RESET32KHZ(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_RESET32KHZ_SHIFT)) & CREG_CREG0_RESET32KHZ_MASK)

/* CREG_CREG0_PD32KHZ 32 kHz power control.
 * 	 0x00 Powered.
 * 	 0x01 Powered-down.
 */
#define CREG_CREG0_PD32KHZ_SHIFT	(3U)
#define CREG_CREG0_PD32KHZ_MASK 	(0x01U << CREG_CREG0_PD32KHZ_SHIFT)
#define CREG_CREG0_PD32KHZ(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_PD32KHZ_SHIFT)) & CREG_CREG0_PD32KHZ_MASK)

/* CREG_CREG0_USB0PHY USB0 PHY power control.
 * 	 0x00 Enable USB0 PHY power.
 * 	 0x01 Disable USB0 PHY. PHY powered down.
 */
#define CREG_CREG0_USB0PHY_SHIFT	(5U)
#define CREG_CREG0_USB0PHY_MASK 	(0x01U << CREG_CREG0_USB0PHY_SHIFT)
#define CREG_CREG0_USB0PHY(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_USB0PHY_SHIFT)) & CREG_CREG0_USB0PHY_MASK)

/* CREG_CREG0_ALARMCTRL RTC_ALARM pin output control
 * 	 0x00 RTC alarm.
 * 	 0x01 Event router event.
 * 	 0x03 Inactive. UM10503 All information provided in this document is subject to legal disclaimers. © NXP B.V. 2012. All rights reserved. NXP Semiconductors UM10503 9.4.2 ARM Cortex-M4 memory mapping register
 */
#define CREG_CREG0_ALARMCTRL_SHIFT	(6U)
#define CREG_CREG0_ALARMCTRL_MASK 	(0x03U << CREG_CREG0_ALARMCTRL_SHIFT)
#define CREG_CREG0_ALARMCTRL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_ALARMCTRL_SHIFT)) & CREG_CREG0_ALARMCTRL_MASK)

/* CREG_CREG0_BODLVL1 BOD trip level to generate an interrupt.See the LPC43xx data sheets for thetrip
 * 	 0x00 Level 0 interrupt
 * 	 0x01 Level 1 interrupt
 * 	 0x02 Level 2 interrupt
 * 	 0x03 Level 3 interrupt
 */
#define CREG_CREG0_BODLVL1_SHIFT	(8U)
#define CREG_CREG0_BODLVL1_MASK 	(0x03U << CREG_CREG0_BODLVL1_SHIFT)
#define CREG_CREG0_BODLVL1(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_BODLVL1_SHIFT)) & CREG_CREG0_BODLVL1_MASK)

/* CREG_CREG0_BODLVL2 BOD trip level to generate a reset. Seethe LPC43xx data sheets for the
 * 	 0x00 Level 0 reset
 * 	 0x01 Level 1 reset
 * 	 0x02 Level 2 reset
 * 	 0x03 Level 3 reset
 */
#define CREG_CREG0_BODLVL2_SHIFT	(10U)
#define CREG_CREG0_BODLVL2_MASK 	(0x03U << CREG_CREG0_BODLVL2_SHIFT)
#define CREG_CREG0_BODLVL2(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_BODLVL2_SHIFT)) & CREG_CREG0_BODLVL2_MASK)

/* CREG_CREG0_SAMPLECTRL SAMPLE pin input/output control
 * 	 0x01 Sample output from the event monitor/recorder.
 */
#define CREG_CREG0_SAMPLECTRL_SHIFT	(12U)
#define CREG_CREG0_SAMPLECTRL_MASK 	(0x03U << CREG_CREG0_SAMPLECTRL_SHIFT)
#define CREG_CREG0_SAMPLECTRL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_SAMPLECTRL_SHIFT)) & CREG_CREG0_SAMPLECTRL_MASK)

/* CREG_CREG0_WAKEUP0CTRL WAKEUP0 pin input/output control
 * 	 0x00 Input to the event router.
 * 	 0x01 Output from the event router.
 * 	 0x03 Input to the event router.
 */
#define CREG_CREG0_WAKEUP0CTRL_SHIFT	(14U)
#define CREG_CREG0_WAKEUP0CTRL_MASK 	(0x03U << CREG_CREG0_WAKEUP0CTRL_SHIFT)
#define CREG_CREG0_WAKEUP0CTRL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_WAKEUP0CTRL_SHIFT)) & CREG_CREG0_WAKEUP0CTRL_MASK)

/* CREG_CREG0_WAKEUP1CTRL WAKEUP1 pin input/output control
 * 	 0x00 Input to event router.
 * 	 0x01 Output from the event router.
 * 	 0x03 Input to event router.
 */
#define CREG_CREG0_WAKEUP1CTRL_SHIFT	(16U)
#define CREG_CREG0_WAKEUP1CTRL_MASK 	(0x03U << CREG_CREG0_WAKEUP1CTRL_SHIFT)
#define CREG_CREG0_WAKEUP1CTRL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG0_WAKEUP1CTRL_SHIFT)) & CREG_CREG0_WAKEUP1CTRL_MASK)

/* M4MEMMAP  at 0x40043100*/
/* CREG_M4MEMMAP_M4MAP Shadow address when accessing memory ataddress
 */
#define CREG_M4MEMMAP_M4MAP_SHIFT	(12U)
#define CREG_M4MEMMAP_M4MAP_MASK 	(0xFFFFFU << CREG_M4MEMMAP_M4MAP_SHIFT)
#define CREG_M4MEMMAP_M4MAP(x)		(((uint32_t)(((uint32_t)(x)) << CREG_M4MEMMAP_M4MAP_SHIFT)) & CREG_M4MEMMAP_M4MAP_MASK)

/* DMAMUX  at 0x4004311C*/
/* CREG_DMAMUX_DMAMUXPER0 Select DMA to peripheral connection for DMA peripheral 0.
 * default: 0
 * 	 0x00 SPIFI
 * 	 0x01 SCT match 2
 * 	 0x03 T3 match 1
 */
#define CREG_DMAMUX_DMAMUXPER0_SHIFT	(0U)
#define CREG_DMAMUX_DMAMUXPER0_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER0_SHIFT)
#define CREG_DMAMUX_DMAMUXPER0(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER0_SHIFT)) & CREG_DMAMUX_DMAMUXPER0_MASK)

/* CREG_DMAMUX_DMAMUXPER1 Select DMA to peripheral connection for DMA peripheral 1
 * default: 0
 * 	 0x00 Timer 0 match 0
 * 	 0x01 USART0 transmit
 */
#define CREG_DMAMUX_DMAMUXPER1_SHIFT	(2U)
#define CREG_DMAMUX_DMAMUXPER1_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER1_SHIFT)
#define CREG_DMAMUX_DMAMUXPER1(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER1_SHIFT)) & CREG_DMAMUX_DMAMUXPER1_MASK)

/* CREG_DMAMUX_DMAMUXPER2 Select DMA to peripheral connection for DMA peripheral 2.
 * default: 0
 * 	 0x00 Timer 0 match 1
 * 	 0x01 USART0 receive
 * 	 0x03 Reserved UM10503 All information provided in this document is subject to legal disclaimers. © NXP B.V. 2012. All rights reserved. NXP Semiconductors UM10503
 */
#define CREG_DMAMUX_DMAMUXPER2_SHIFT	(4U)
#define CREG_DMAMUX_DMAMUXPER2_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER2_SHIFT)
#define CREG_DMAMUX_DMAMUXPER2(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER2_SHIFT)) & CREG_DMAMUX_DMAMUXPER2_MASK)

/* CREG_DMAMUX_DMAMUXPER3 Select DMA to peripheral connection for DMA peripheral 3.
 * default: 0
 * 	 0x00 Timer 1 match 0
 * 	 0x01 UART1 transmit
 * 	 0x02 I2S1 DMA request 1
 * 	 0x03 SSP1 transmit
 */
#define CREG_DMAMUX_DMAMUXPER3_SHIFT	(6U)
#define CREG_DMAMUX_DMAMUXPER3_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER3_SHIFT)
#define CREG_DMAMUX_DMAMUXPER3(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER3_SHIFT)) & CREG_DMAMUX_DMAMUXPER3_MASK)

/* CREG_DMAMUX_DMAMUXPER4 Select DMA to peripheral connection for DMA peripheral 4.
 * default: 0
 * 	 0x00 Timer 1 match 1
 * 	 0x01 UART1 receive
 * 	 0x02 I2S1 DMA request 2
 * 	 0x03 SSP1 receive
 */
#define CREG_DMAMUX_DMAMUXPER4_SHIFT	(8U)
#define CREG_DMAMUX_DMAMUXPER4_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER4_SHIFT)
#define CREG_DMAMUX_DMAMUXPER4(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER4_SHIFT)) & CREG_DMAMUX_DMAMUXPER4_MASK)

/* CREG_DMAMUX_DMAMUXPER5 Select DMA to peripheral connection for DMA peripheral 5.
 * default: 0
 * 	 0x00 Timer 2 match 0
 * 	 0x01 USART2 transmit
 * 	 0x02 SSP1 transmit
 */
#define CREG_DMAMUX_DMAMUXPER5_SHIFT	(10U)
#define CREG_DMAMUX_DMAMUXPER5_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER5_SHIFT)
#define CREG_DMAMUX_DMAMUXPER5(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER5_SHIFT)) & CREG_DMAMUX_DMAMUXPER5_MASK)

/* CREG_DMAMUX_DMAMUXPER6 Selects DMA to peripheral connection for DMA peripheral 6.
 * default: 0
 * 	 0x00 Timer 2 match 1
 * 	 0x01 USART2 receive
 * 	 0x02 SSP1 receive
 */
#define CREG_DMAMUX_DMAMUXPER6_SHIFT	(12U)
#define CREG_DMAMUX_DMAMUXPER6_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER6_SHIFT)
#define CREG_DMAMUX_DMAMUXPER6(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER6_SHIFT)) & CREG_DMAMUX_DMAMUXPER6_MASK)

/* CREG_DMAMUX_DMAMUXPER7 Selects DMA to peripheral connection for DMA peripheral 7.
 * default: 0
 * 	 0x00 Timer 3 match l 0
 * 	 0x01 USART3 transmit
 * 	 0x02 SCT match output 0
 */
#define CREG_DMAMUX_DMAMUXPER7_SHIFT	(14U)
#define CREG_DMAMUX_DMAMUXPER7_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER7_SHIFT)
#define CREG_DMAMUX_DMAMUXPER7(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER7_SHIFT)) & CREG_DMAMUX_DMAMUXPER7_MASK)

/* CREG_DMAMUX_DMAMUXPER8 Select DMA to peripheral connection for DMA peripheral 8.
 * default: 0
 * 	 0x00 Timer 3 match 1
 * 	 0x01 USART3 receive
 * 	 0x02 SCT match output 1
 */
#define CREG_DMAMUX_DMAMUXPER8_SHIFT	(16U)
#define CREG_DMAMUX_DMAMUXPER8_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER8_SHIFT)
#define CREG_DMAMUX_DMAMUXPER8(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER8_SHIFT)) & CREG_DMAMUX_DMAMUXPER8_MASK)

/* CREG_DMAMUX_DMAMUXPER9 Select DMA to peripheral connection for DMA peripheral 9.
 * default: 0
 * 	 0x00 SSP0 receive
 * 	 0x01 I2S0 DMA request 1
 * 	 0x02 SCT match output 1
 * 	 0x03 Reserved Table 45. DMA mux control register (DMAMUX, address 0x4004 311C) bit description value UM10503 All information provided in this document is subject to legal disclaimers. © NXP B.V. 2012. All rights reserved. NXP Semiconductors UM10503 9.4.5 Flash Accelerator Configuration register for flash bank A Following reset, flash accelerator functions are enabled and flash access timing is set to a
 */
#define CREG_DMAMUX_DMAMUXPER9_SHIFT	(18U)
#define CREG_DMAMUX_DMAMUXPER9_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER9_SHIFT)
#define CREG_DMAMUX_DMAMUXPER9(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER9_SHIFT)) & CREG_DMAMUX_DMAMUXPER9_MASK)

/* CREG_DMAMUX_DMAMUXPER10 Select DMA to peripheral connection for DMA peripheral 10.
 * default: 0
 * 	 0x00 SSP0 transmit
 * 	 0x01 I2S0 DMA request 2
 * 	 0x02 SCT match output 0
 */
#define CREG_DMAMUX_DMAMUXPER10_SHIFT	(20U)
#define CREG_DMAMUX_DMAMUXPER10_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER10_SHIFT)
#define CREG_DMAMUX_DMAMUXPER10(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER10_SHIFT)) & CREG_DMAMUX_DMAMUXPER10_MASK)

/* CREG_DMAMUX_DMAMUXPER11 Selects DMA to peripheral connection for DMA peripheral 11.
 * default: 0
 * 	 0x00 SSP1 receive
 * 	 0x02 USART0 transmit
 */
#define CREG_DMAMUX_DMAMUXPER11_SHIFT	(22U)
#define CREG_DMAMUX_DMAMUXPER11_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER11_SHIFT)
#define CREG_DMAMUX_DMAMUXPER11(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER11_SHIFT)) & CREG_DMAMUX_DMAMUXPER11_MASK)

/* CREG_DMAMUX_DMAMUXPER12 Select DMA to peripheral connection for DMA peripheral 12.
 * default: 0
 * 	 0x00 SSP1 transmit
 * 	 0x02 USART0 receive
 */
#define CREG_DMAMUX_DMAMUXPER12_SHIFT	(24U)
#define CREG_DMAMUX_DMAMUXPER12_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER12_SHIFT)
#define CREG_DMAMUX_DMAMUXPER12(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER12_SHIFT)) & CREG_DMAMUX_DMAMUXPER12_MASK)

/* CREG_DMAMUX_DMAMUXPER13 Select DMA to peripheral connection for DMA peripheral 13.
 * default: 0
 * 	 0x00 ADC0
 * 	 0x02 SSP1 receive
 * 	 0x03 USART3 receive
 */
#define CREG_DMAMUX_DMAMUXPER13_SHIFT	(26U)
#define CREG_DMAMUX_DMAMUXPER13_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER13_SHIFT)
#define CREG_DMAMUX_DMAMUXPER13(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER13_SHIFT)) & CREG_DMAMUX_DMAMUXPER13_MASK)

/* CREG_DMAMUX_DMAMUXPER14 Select DMA to peripheral connection for DMA peripheral 14.
 * default: 0
 * 	 0x00 ADC1
 * 	 0x02 SSP1 transmit
 * 	 0x03 USART3 transmit
 */
#define CREG_DMAMUX_DMAMUXPER14_SHIFT	(28U)
#define CREG_DMAMUX_DMAMUXPER14_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER14_SHIFT)
#define CREG_DMAMUX_DMAMUXPER14(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER14_SHIFT)) & CREG_DMAMUX_DMAMUXPER14_MASK)

/* CREG_DMAMUX_DMAMUXPER15 Select DMA to peripheral connection for DMA peripheral 15.
 * default: 0
 * 	 0x00 DAC
 * 	 0x01 SCT match output 3
 * 	 0x03 Timer 3 match 0
 */
#define CREG_DMAMUX_DMAMUXPER15_SHIFT	(30U)
#define CREG_DMAMUX_DMAMUXPER15_MASK 	(0x03U << CREG_DMAMUX_DMAMUXPER15_SHIFT)
#define CREG_DMAMUX_DMAMUXPER15(x)		(((uint32_t)(((uint32_t)(x)) << CREG_DMAMUX_DMAMUXPER15_SHIFT)) & CREG_DMAMUX_DMAMUXPER15_MASK)

/* ETBCFG  at 0x40043128*/
/* CREG_ETBCFG_ETB Select SRAM interface
 * default: 1
 * 	 0x00 ETB accesses SRAM at address 0x2000 C000.
 * 	 0x01 AHB accesses SRAM at address 0x2000 C000.
 */
#define CREG_ETBCFG_ETB_SHIFT	(0U)
#define CREG_ETBCFG_ETB_MASK 	(0x01U << CREG_ETBCFG_ETB_SHIFT)
#define CREG_ETBCFG_ETB(x)		(((uint32_t)(((uint32_t)(x)) << CREG_ETBCFG_ETB_SHIFT)) & CREG_ETBCFG_ETB_MASK)

/* CREG6  at 0x4004312C*/
/* CREG_CREG6_ETHMODE Selects the Ethernet mode. Reset the ethernet after changing the PHY interface. All other settings are
 * default: reserved.
 * 	 0x00 MII
 * 	 0x04 RMII
 */
#define CREG_CREG6_ETHMODE_SHIFT	(0U)
#define CREG_CREG6_ETHMODE_MASK 	(0x07U << CREG_CREG6_ETHMODE_SHIFT)
#define CREG_CREG6_ETHMODE(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_ETHMODE_SHIFT)) & CREG_CREG6_ETHMODE_MASK)

/* CREG_CREG6_CTOUTCTRL Selects the functionality of the SCT outputs.
 * default: 0
 * 	 0x00 Combine SCT and timer match outputs. SCT outputs are ORed with timer outputs. outputs.
 */
#define CREG_CREG6_CTOUTCTRL_SHIFT	(4U)
#define CREG_CREG6_CTOUTCTRL_MASK 	(0x01U << CREG_CREG6_CTOUTCTRL_SHIFT)
#define CREG_CREG6_CTOUTCTRL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_CTOUTCTRL_SHIFT)) & CREG_CREG6_CTOUTCTRL_MASK)

/* CREG_CREG6_I2S0_TX_SCK_IN_SEL I2S0_TX_SCK input select
 * default: 0
 * 	 0x00 I2S clock selected as defined by the I2S transmit mode register Table 909. and MCLK output. The I2S must be configured in slave mode.
 */
#define CREG_CREG6_I2S0_TX_SCK_IN_SEL_SHIFT	(12U)
#define CREG_CREG6_I2S0_TX_SCK_IN_SEL_MASK 	(0x01U << CREG_CREG6_I2S0_TX_SCK_IN_SEL_SHIFT)
#define CREG_CREG6_I2S0_TX_SCK_IN_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_I2S0_TX_SCK_IN_SEL_SHIFT)) & CREG_CREG6_I2S0_TX_SCK_IN_SEL_MASK)

/* CREG_CREG6_I2S0_RX_SCK_IN_SEL I2S0_RX_SCK input select
 * default: 0
 * 	 0x00 I2S clock selected as defined by the I2S receive mode register Table 910. and MCLK output. The I2S must be configured in slave mode.
 */
#define CREG_CREG6_I2S0_RX_SCK_IN_SEL_SHIFT	(13U)
#define CREG_CREG6_I2S0_RX_SCK_IN_SEL_MASK 	(0x01U << CREG_CREG6_I2S0_RX_SCK_IN_SEL_SHIFT)
#define CREG_CREG6_I2S0_RX_SCK_IN_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_I2S0_RX_SCK_IN_SEL_SHIFT)) & CREG_CREG6_I2S0_RX_SCK_IN_SEL_MASK)

/* CREG_CREG6_I2S1_TX_SCK_IN_SEL I2S1_TX_SCK input select
 * default: 0
 * 	 0x00 I2 S clock selected as defined by the I2S transmit mode register Table 909. and MCLK output. The I2S must be configured in slave mode.
 */
#define CREG_CREG6_I2S1_TX_SCK_IN_SEL_SHIFT	(14U)
#define CREG_CREG6_I2S1_TX_SCK_IN_SEL_MASK 	(0x01U << CREG_CREG6_I2S1_TX_SCK_IN_SEL_SHIFT)
#define CREG_CREG6_I2S1_TX_SCK_IN_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_I2S1_TX_SCK_IN_SEL_SHIFT)) & CREG_CREG6_I2S1_TX_SCK_IN_SEL_MASK)

/* CREG_CREG6_I2S1_RX_SCK_IN_SEL I2S1_RX_SCK input select
 * default: 0
 * 	 0x00 I2 S clock selected as defined by the I2S receive mode register Table 910. and MCLK output. The I2S must be configured in slave mode.
 */
#define CREG_CREG6_I2S1_RX_SCK_IN_SEL_SHIFT	(15U)
#define CREG_CREG6_I2S1_RX_SCK_IN_SEL_MASK 	(0x01U << CREG_CREG6_I2S1_RX_SCK_IN_SEL_SHIFT)
#define CREG_CREG6_I2S1_RX_SCK_IN_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_I2S1_RX_SCK_IN_SEL_SHIFT)) & CREG_CREG6_I2S1_RX_SCK_IN_SEL_MASK)

/* CREG_CREG6_EMC_CLK_SEL EMC_CLK divided clock select (see Section 21.1).
 * default: 0
 * 	 0x00 EMC_CLK_DIV not divided.
 * 	 0x01 EMC_CLK_DIV divided by 2.
 */
#define CREG_CREG6_EMC_CLK_SEL_SHIFT	(16U)
#define CREG_CREG6_EMC_CLK_SEL_MASK 	(0x01U << CREG_CREG6_EMC_CLK_SEL_SHIFT)
#define CREG_CREG6_EMC_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CREG6_EMC_CLK_SEL_SHIFT)) & CREG_CREG6_EMC_CLK_SEL_MASK)

/* M4TXEVENT  at 0x40043130*/
/* CREG_M4TXEVENT_TXEVCLR Cortex-M4 TXEV event.
 * default: 0
 * 	 0x00 Clear the TXEV event.
 * 	 0x01 No effect.
 */
#define CREG_M4TXEVENT_TXEVCLR_SHIFT	(0U)
#define CREG_M4TXEVENT_TXEVCLR_MASK 	(0x01U << CREG_M4TXEVENT_TXEVCLR_SHIFT)
#define CREG_M4TXEVENT_TXEVCLR(x)		(((uint32_t)(((uint32_t)(x)) << CREG_M4TXEVENT_TXEVCLR_SHIFT)) & CREG_M4TXEVENT_TXEVCLR_MASK)

/* CHIPID  at 0x40043200*/
/* CREG_CHIPID_ID Boundary scan ID code 0x5906 002B or 0x6906 002B = LPC4350/30/20/10 (flashless parts) 0x4906 002B = LPC4357/53 (parts with on-chip flash)
 * default: -
 */
#define CREG_CHIPID_ID_SHIFT	(0U)
#define CREG_CHIPID_ID_MASK 	(0xFFFFFFFFU << CREG_CHIPID_ID_SHIFT)
#define CREG_CHIPID_ID(x)		(((uint32_t)(((uint32_t)(x)) << CREG_CHIPID_ID_SHIFT)) & CREG_CHIPID_ID_MASK)

/* M0TXEVENT  at 0x40043400*/
/* CREG_M0TXEVENT_TXEVCLR Cortex-M0 TXEV event.
 * default: 0
 * 	 0x00 Clear the TXEV event.
 * 	 0x01 No effect.
 */
#define CREG_M0TXEVENT_TXEVCLR_SHIFT	(0U)
#define CREG_M0TXEVENT_TXEVCLR_MASK 	(0x01U << CREG_M0TXEVENT_TXEVCLR_SHIFT)
#define CREG_M0TXEVENT_TXEVCLR(x)		(((uint32_t)(((uint32_t)(x)) << CREG_M0TXEVENT_TXEVCLR_SHIFT)) & CREG_M0TXEVENT_TXEVCLR_MASK)

/* M0APPMEMMAP  at 0x40043404*/
/* CREG_M0APPMEMMAP_M0APPMAP Shadow address when accessing memory at address 0x0000 0000 0x2000
 * default: 0000
 */
#define CREG_M0APPMEMMAP_M0APPMAP_SHIFT	(12U)
#define CREG_M0APPMEMMAP_M0APPMAP_MASK 	(0xFFFFFU << CREG_M0APPMEMMAP_M0APPMAP_SHIFT)
#define CREG_M0APPMEMMAP_M0APPMAP(x)		(((uint32_t)(((uint32_t)(x)) << CREG_M0APPMEMMAP_M0APPMAP_SHIFT)) & CREG_M0APPMEMMAP_M0APPMAP_MASK)

/* M0APPMEMMAP  at 0x40043404*/
/* CREG_M0APPMEMMAP_FLTV Frame length timing value The frame length is given in the number of high-speed bit times in decimal format.
 * Each decimal value change to this register corresponds to 16 high-speed bit times. The SOF cycle time (number of SOF counter clock
 * periods to generate a SOF micro-frame length) is equal to 59488 + value in this field. The default value is decimal 32 (0x20), which
 * results in a SOF cycle time of 60000. 0x00 = 59488 (= 59488 + 0 x 16) 0x01 = 59504 (= 59488 + 1 x 16) 0x02 = 59520 (= 59488 + 2 x 16) ...
 * 0x1F = 59984 (= 59488 + 31 x 16) 0x20 = 60000 (= 59488 + 32 x 16) ... 0x3E = 60480 (= 59488 + 62 x 16) 0x3F = 60496 (= 59488 + 63 x 16)
 * default: 0x20
 */
#define CREG_M0APPMEMMAP_FLTV_SHIFT	(0U)
#define CREG_M0APPMEMMAP_FLTV_MASK 	(0x3FU << CREG_M0APPMEMMAP_FLTV_SHIFT)
#define CREG_M0APPMEMMAP_FLTV(x)		(((uint32_t)(((uint32_t)(x)) << CREG_M0APPMEMMAP_FLTV_SHIFT)) & CREG_M0APPMEMMAP_FLTV_MASK)

/* USB1FLADJ   at * 0x40043600/
/* CREG__FLTV Frame length timing value The frame length is given in the number of high-speed bit times in decimal format. Each decimal value change to this register corresponds to 16 high-speed bit times. The SOF cycle time (number of SOF counter clock periods to generate a SOF micro-frame length) is equal to 59488 + value in this field. The default value is decimal 32 (0x20), which results in a SOF cycle time of 60000. 0x00 = 59488 (= 59488 + 0 x 16) 0x01 = 59504 (= 59488 + 1 x 16) 0x02 = 59520 (= 59488 + 2 x 16) ... 0x1F = 59984 (= 59488 + 31 x 16) 0x20 = 60000 (= 59488 + 32 x 16) ... 0x3E = 60480 (= 59488 + 62 x 16) 0x3F = 60496 (= 59488 + 63 x 16)
 * default: 0x20
 */
#define CREG__FLTV_SHIFT	(0U)
#define CREG__FLTV_MASK 	(0x3FU << CREG__FLTV_SHIFT)
#define CREG__FLTV(x)		(((uint32_t)(((uint32_t)(x)) << CREG__FLTV_SHIFT)) & CREG__FLTV_MASK)

/** @defgroup CREG_18XX_43XX CHIP: LPC18xx/43xx CREG driver
 * @ingroup CHIP_18XX_43XX_Drivers
 * @{
 */

/**
 * @brief CREG Register Block
 */
typedef struct {						/*!< CREG Structure         */
	__I  uint32_t  RESERVED0;
	__IO uint32_t  CREG0;				/*!< Chip configuration register 32 kHz oscillator output and BOD control register. */
	__I  uint32_t  RESERVED1[62];
	__IO uint32_t  MXMEMMAP;			/*!< ARM Cortex-M3/M4 memory mapping */
#if defined(CHIP_LPC18XX)
	__I  uint32_t  RESERVED2[5];
#else
	__I  uint32_t  RESERVED2;
	__I  uint32_t  CREG1;				/*!< Configuration Register 1 */
	__I  uint32_t  CREG2;				/*!< Configuration Register 2 */
	__I  uint32_t  CREG3;				/*!< Configuration Register 3 */
	__I  uint32_t  CREG4;				/*!< Configuration Register 4 */
#endif
	__IO uint32_t  CREG5;				/*!< Chip configuration register 5. Controls JTAG access. */
	__IO uint32_t  DMAMUX;				/*!< DMA muxing control     */
	__IO uint32_t  FLASHCFGA;			/*!< Flash accelerator configuration register for flash bank A */
	__IO uint32_t  FLASHCFGB;			/*!< Flash accelerator configuration register for flash bank B */
	__IO uint32_t  ETBCFG;				/*!< ETB RAM configuration  */
	__IO uint32_t  CREG6;				/*!< Chip configuration register 6. */
#if defined(CHIP_LPC18XX)
	__I  uint32_t  RESERVED4[52];
#else
	__IO uint32_t  M4TXEVENT;			/*!< M4 IPC event register */
	__I  uint32_t  RESERVED4[51];
#endif
	__I  uint32_t  CHIPID;				/*!< Part ID                */
#if defined(CHIP_LPC18XX)
	__I  uint32_t  RESERVED5[191];
#else
	__I  uint32_t  RESERVED5[65];
	__IO uint32_t  M0SUBMEMMAP;         /*!< M0SUB IPC Event memory mapping */
	__I  uint32_t  RESERVED6[2];
	__IO uint32_t  M0SUBTXEVENT;        /*!< M0SUB IPC Event register */
	__I  uint32_t  RESERVED7[58];
	__IO uint32_t  M0APPTXEVENT;		/*!< M0APP IPC Event register */
	__IO uint32_t  M0APPMEMMAP;			/*!< ARM Cortex M0APP memory mapping */
	__I  uint32_t  RESERVED8[62];
#endif
	__IO uint32_t  USB0FLADJ;			/*!< USB0 frame length adjust register */
	__I  uint32_t  RESERVED9[63];
	__IO uint32_t  USB1FLADJ;			/*!< USB1 frame length adjust register */
} LPC_CREG_T;

/**
 * @brief	Identifies whether on-chip flash is present
 * @return	true if on chip flash is available, otherwise false
 */
STATIC INLINE uint32_t Chip_CREG_OnChipFlashIsPresent(void)
{
	return LPC_CREG->CHIPID != 0x3284E02B;
}

/**
 * @brief	Configures the onboard Flash Accelerator in flash-based LPC18xx/LPC43xx parts.
 * @param	Hz	: Current frequency in Hz of the CPU
 * @return	Nothing
 * This function should be called with the higher frequency before the clock frequency is
 * increased and it should be called with the new lower value after the clock frequency is
 * decreased.
 */
STATIC INLINE void Chip_CREG_SetFlashAcceleration(uint32_t Hz)
{
	uint32_t FAValue = Hz / 21510000;

	LPC_CREG->FLASHCFGA = (LPC_CREG->FLASHCFGA & (~(0xF << 12))) | (FAValue << 12);
	LPC_CREG->FLASHCFGB = (LPC_CREG->FLASHCFGB & (~(0xF << 12))) | (FAValue << 12);
}

/**
 * @brief FLASH Access time definitions
 */
typedef enum {
	FLASHTIM_20MHZ_CPU = 0,		/*!< Flash accesses use 1 CPU clocks. Use for up to 20 MHz CPU clock */
	FLASHTIM_40MHZ_CPU = 1,		/*!< Flash accesses use 2 CPU clocks. Use for up to 40 MHz CPU clock */
	FLASHTIM_60MHZ_CPU = 2,		/*!< Flash accesses use 3 CPU clocks. Use for up to 60 MHz CPU clock */
	FLASHTIM_80MHZ_CPU = 3,		/*!< Flash accesses use 4 CPU clocks. Use for up to 80 MHz CPU clock */
	FLASHTIM_100MHZ_CPU = 4,	/*!< Flash accesses use 5 CPU clocks. Use for up to 100 MHz CPU clock */
	FLASHTIM_120MHZ_CPU = 5,	/*!< Flash accesses use 6 CPU clocks. Use for up to 120 MHz CPU clock */
	FLASHTIM_150MHZ_CPU = 6,	/*!< Flash accesses use 7 CPU clocks. Use for up to 150 Mhz CPU clock */
	FLASHTIM_170MHZ_CPU = 7,		/*!< Flash accesses use 8 CPU clocks. Use for up to 170 MHz CPU clock */
	FLASHTIM_190MHZ_CPU = 8,		/*!< Flash accesses use 9 CPU clocks. Use for up to 190 MHz CPU clock */
	FLASHTIM_SAFE_SETTING = 9,		/*!< Flash accesses use 10 CPU clocks. Safe setting for any allowed conditions */
} CREG_FLASHTIM_T;

/**
 * @brief	Set FLASH memory access time in clocks
 * @param	clks	: FLASH access speed rating
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_SetFLASHAccess(CREG_FLASHTIM_T clks)
{
	uint32_t tmpA, tmpB;

	/* Don't alter lower bits */
	tmpA = LPC_CREG->FLASHCFGA & ~(0xF << 12);
	LPC_CREG->FLASHCFGA = tmpA | ((uint32_t) clks << 12);
	tmpB = LPC_CREG->FLASHCFGB & ~(0xF << 12);
	LPC_CREG->FLASHCFGB = tmpB | ((uint32_t) clks << 12);
}

/**
 * @brief	Enables the USB0 high-speed PHY on LPC18xx/LPC43xx parts
 * @return	Nothing
 * @note	The USB0 PLL & clock should be configured before calling this function. This function
 * should be called before the USB0 registers are accessed.
 */
STATIC INLINE void Chip_CREG_EnableUSB0Phy(void)
{
	LPC_CREG->CREG0 &= ~(1 << 5);
}

/**
 * @brief	Disable the USB0 high-speed PHY on LPC18xx/LPC43xx parts
 * @return	Nothing
 * @note	The USB0 PLL & clock should be configured before calling this function. This function
 * should be called before the USB0 registers are accessed.
 */
STATIC INLINE void Chip_CREG_DisableUSB0Phy(void)
{
	LPC_CREG->CREG0 |= (1 << 5);
}

/**
 * @brief	Configures the BOD and Reset on LPC18xx/LPC43xx parts.
 * @param	BODVL	: Brown-Out Detect voltage level (0-3)
 * @param	BORVL	: Brown-Out Reset voltage level (0-3)
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_ConfigureBODaR(uint32_t BODVL, uint32_t BORVL)
{
	LPC_CREG->CREG0 = (LPC_CREG->CREG0 & ~((3 << 8) | (3 << 10))) | (BODVL << 8) | (BORVL << 10);
}

#if (defined(CHIP_LPC43XX) && defined(LPC_CREG))
/**
 * @brief	Configures base address of image to be run in the Cortex M0APP Core.
 * @param	memaddr	: Address of the image (must be aligned to 4K)
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_SetM0AppMemMap(uint32_t memaddr)
{
	LPC_CREG->M0APPMEMMAP = memaddr & ~0xFFF;
}

/**
 * @brief	Configures base address of image to be run in the Cortex M0SUB Core.
 * @param	memaddr	: Address of the image (must be aligned to 4K)
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_SetM0SubMemMap(uint32_t memaddr)
{
	LPC_CREG->M0SUBMEMMAP = memaddr & ~0xFFF;
}

/**
 * @brief	Clear M4 IPC Event
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_ClearM4Event(void)
{
	LPC_CREG->M4TXEVENT = 0;
}

/**
 * @brief	Clear M0APP IPC Event
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_ClearM0AppEvent(void)
{
	LPC_CREG->M0APPTXEVENT = 0;
}

/**
 * @brief	Clear M0APP IPC Event
 * @return	Nothing
 */
STATIC INLINE void Chip_CREG_ClearM0SubEvent(void)
{
	LPC_CREG->M0SUBTXEVENT = 0;
}
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CREG_18XX_43XX_H_ */






