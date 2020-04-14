/*
 * @brief CGU/CCU registers and control functions
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

#ifndef __CGUCCU_18XX_43XX_H_
#define __CGUCCU_18XX_43XX_H_

#include "chip_clocks.h"

#ifdef __cplusplus
extern "C" {
#endif

/* FREQ_MON  at 0x40050014*/
/* CGU_FREQ_MON_RCNT 9-bit reference clock-counter value
 * default: 0
 */
#define CGU_FREQ_MON_RCNT_SHIFT	(0U)
#define CGU_FREQ_MON_RCNT_MASK 	(0x1FFU << CGU_FREQ_MON_RCNT_SHIFT)
#define CGU_FREQ_MON_RCNT(x)		(((uint32_t)(((uint32_t)(x)) << CGU_FREQ_MON_RCNT_SHIFT)) & CGU_FREQ_MON_RCNT_MASK)

/* CGU_FREQ_MON_FCNT 14-bit selected clock-counter value
 * default: 0
 */
#define CGU_FREQ_MON_FCNT_SHIFT	(9U)
#define CGU_FREQ_MON_FCNT_MASK 	(0x3FFFU << CGU_FREQ_MON_FCNT_SHIFT)
#define CGU_FREQ_MON_FCNT(x)		(((uint32_t)(((uint32_t)(x)) << CGU_FREQ_MON_FCNT_SHIFT)) & CGU_FREQ_MON_FCNT_MASK)

/* CGU_FREQ_MON_MEAS Measure frequency
 * default: 0
 * 	 0x00 RCNT and FCNT disabled
 * 	 0x01 Frequency counters started
 */
#define CGU_FREQ_MON_MEAS_SHIFT	(23U)
#define CGU_FREQ_MON_MEAS_MASK 	(0x01U << CGU_FREQ_MON_MEAS_SHIFT)
#define CGU_FREQ_MON_MEAS(x)		(((uint32_t)(((uint32_t)(x)) << CGU_FREQ_MON_MEAS_SHIFT)) & CGU_FREQ_MON_MEAS_MASK)

/* CGU_FREQ_MON_CLK_SEL Clock-source selection for the clock to be measured. All other values are reserved.
 * default: 0
 * 	 0x00 32 kHz oscillator (default)
 * 	 0x01 IRC
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_FREQ_MON_CLK_SEL_SHIFT	(24U)
#define CGU_FREQ_MON_CLK_SEL_MASK 	(0x1FU << CGU_FREQ_MON_CLK_SEL_SHIFT)
#define CGU_FREQ_MON_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_FREQ_MON_CLK_SEL_SHIFT)) & CGU_FREQ_MON_CLK_SEL_MASK)

/* XTAL_OSC_CTRL  at 0x40050018*/
/* CGU_XTAL_OSC_CTRL_ENABLE Oscillator-pad enable. Do not change the BYPASS and ENABLE bits in one write-action: this will result in unstable device operation!
 * default: 1
 * 	 0x00 Enable
 * 	 0x01 Power-down (default) UM10503 All information provided in this document is subject to legal disclaimers. © NXP B.V. 2012. All rights reserved. NXP Semiconductors UM10503 11.6.3 PLL0USB registers See Section 11.7.4.5 for instructions on how to set up the PLL0USB. 11.6.3.2 PLL0USB control register
 */
#define CGU_XTAL_OSC_CTRL_ENABLE_SHIFT	(0U)
#define CGU_XTAL_OSC_CTRL_ENABLE_MASK 	(0x01U << CGU_XTAL_OSC_CTRL_ENABLE_SHIFT)
#define CGU_XTAL_OSC_CTRL_ENABLE(x)		(((uint32_t)(((uint32_t)(x)) << CGU_XTAL_OSC_CTRL_ENABLE_SHIFT)) & CGU_XTAL_OSC_CTRL_ENABLE_MASK)

/* CGU_XTAL_OSC_CTRL_BYPASS Configure crystal operation or external-clock input pin XTAL1. Do not change the BYPASS and ENABLE bits in one write-action: this will result in unstable device operation!
 * default: 0
 * 	 0x00 Operation with crystal connected (default).
 * 	 0x01 Bypass mode. Use this mode when an external clock source is used instead of a crystal.
 */
#define CGU_XTAL_OSC_CTRL_BYPASS_SHIFT	(1U)
#define CGU_XTAL_OSC_CTRL_BYPASS_MASK 	(0x01U << CGU_XTAL_OSC_CTRL_BYPASS_SHIFT)
#define CGU_XTAL_OSC_CTRL_BYPASS(x)		(((uint32_t)(((uint32_t)(x)) << CGU_XTAL_OSC_CTRL_BYPASS_SHIFT)) & CGU_XTAL_OSC_CTRL_BYPASS_MASK)

/* CGU_XTAL_OSC_CTRL_HF Select frequency range
 * default: 1
 * 	 0x00 Oscillator low-frequency mode (crystal or external clock source 1 to 20 MHz). Between 15 MHz and
 * 	 0x01 Oscillator high-frequency mode; crystal or external clock source 15 to 25 MHz. Between 15 MHz and
 */
#define CGU_XTAL_OSC_CTRL_HF_SHIFT	(2U)
#define CGU_XTAL_OSC_CTRL_HF_MASK 	(0x01U << CGU_XTAL_OSC_CTRL_HF_SHIFT)
#define CGU_XTAL_OSC_CTRL_HF(x)		(((uint32_t)(((uint32_t)(x)) << CGU_XTAL_OSC_CTRL_HF_SHIFT)) & CGU_XTAL_OSC_CTRL_HF_MASK)

/* PLL0USB_STAT  at 0x4005001C*/
/* CGU_PLL0USB_STAT_LOCK PLL0 lock indicator
 * default: 0
 */
#define CGU_PLL0USB_STAT_LOCK_SHIFT	(0U)
#define CGU_PLL0USB_STAT_LOCK_MASK 	(0x01U << CGU_PLL0USB_STAT_LOCK_SHIFT)
#define CGU_PLL0USB_STAT_LOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_STAT_LOCK_SHIFT)) & CGU_PLL0USB_STAT_LOCK_MASK)

/* CGU_PLL0USB_STAT_FR PLL0 free running indicator
 * default: 0
 */
#define CGU_PLL0USB_STAT_FR_SHIFT	(1U)
#define CGU_PLL0USB_STAT_FR_MASK 	(0x01U << CGU_PLL0USB_STAT_FR_SHIFT)
#define CGU_PLL0USB_STAT_FR(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_STAT_FR_SHIFT)) & CGU_PLL0USB_STAT_FR_MASK)

/* PLL0USB_CTRL  at 0x40050020*/
/* CGU_PLL0USB_CTRL_PD PLL0 power down
 * default: 1
 * 	 0x00 PLL0 enabled
 * 	 0x01 PLL0 powered down
 */
#define CGU_PLL0USB_CTRL_PD_SHIFT	(0U)
#define CGU_PLL0USB_CTRL_PD_MASK 	(0x01U << CGU_PLL0USB_CTRL_PD_SHIFT)
#define CGU_PLL0USB_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_PD_SHIFT)) & CGU_PLL0USB_CTRL_PD_MASK)

/* CGU_PLL0USB_CTRL_BYPASS Input clock bypass control
 * default: 1
 * 	 0x00 CCO clock sent to post-dividers. Use this in normal operation. (default).
 */
#define CGU_PLL0USB_CTRL_BYPASS_SHIFT	(1U)
#define CGU_PLL0USB_CTRL_BYPASS_MASK 	(0x01U << CGU_PLL0USB_CTRL_BYPASS_SHIFT)
#define CGU_PLL0USB_CTRL_BYPASS(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_BYPASS_SHIFT)) & CGU_PLL0USB_CTRL_BYPASS_MASK)

/* CGU_PLL0USB_CTRL_DIRECTI PLL0 direct input
 * default: 0
 */
#define CGU_PLL0USB_CTRL_DIRECTI_SHIFT	(2U)
#define CGU_PLL0USB_CTRL_DIRECTI_MASK 	(0x01U << CGU_PLL0USB_CTRL_DIRECTI_SHIFT)
#define CGU_PLL0USB_CTRL_DIRECTI(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_DIRECTI_SHIFT)) & CGU_PLL0USB_CTRL_DIRECTI_MASK)

/* CGU_PLL0USB_CTRL_DIRECTO PLL0 direct output
 * default: 0
 */
#define CGU_PLL0USB_CTRL_DIRECTO_SHIFT	(3U)
#define CGU_PLL0USB_CTRL_DIRECTO_MASK 	(0x01U << CGU_PLL0USB_CTRL_DIRECTO_SHIFT)
#define CGU_PLL0USB_CTRL_DIRECTO(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_DIRECTO_SHIFT)) & CGU_PLL0USB_CTRL_DIRECTO_MASK)

/* CGU_PLL0USB_CTRL_CLKEN PLL0 clock enable
 * default: 0
 */
#define CGU_PLL0USB_CTRL_CLKEN_SHIFT	(4U)
#define CGU_PLL0USB_CTRL_CLKEN_MASK 	(0x01U << CGU_PLL0USB_CTRL_CLKEN_SHIFT)
#define CGU_PLL0USB_CTRL_CLKEN(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_CLKEN_SHIFT)) & CGU_PLL0USB_CTRL_CLKEN_MASK)

/* CGU_PLL0USB_CTRL_FRM Free running mode
 * default: 0
 * 	 0x08 Reserved one to this register.
 * 	 0x09 Reserved one to this register.
 * 	 0x00 Reserved one to this register.
 */
#define CGU_PLL0USB_CTRL_FRM_SHIFT	(6U)
#define CGU_PLL0USB_CTRL_FRM_MASK 	(0x01U << CGU_PLL0USB_CTRL_FRM_SHIFT)
#define CGU_PLL0USB_CTRL_FRM(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_FRM_SHIFT)) & CGU_PLL0USB_CTRL_FRM_MASK)

/* CGU_PLL0USB_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_PLL0USB_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_PLL0USB_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_PLL0USB_CTRL_AUTOBLOCK_SHIFT)
#define CGU_PLL0USB_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_AUTOBLOCK_SHIFT)) & CGU_PLL0USB_CTRL_AUTOBLOCK_MASK)

/* CGU_PLL0USB_CTRL_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_PLL0USB_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_PLL0USB_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_PLL0USB_CTRL_CLK_SEL_SHIFT)
#define CGU_PLL0USB_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_CTRL_CLK_SEL_SHIFT)) & CGU_PLL0USB_CTRL_CLK_SEL_MASK)

/* PLL0USB_MDIV  at 0x40050024*/
/* CGU_PLL0USB_MDIV_MDEC Decoded M-divider coefficient value. Select values for the M-divider between 1 and 131071.
 * default: 0x5B6A
 */
#define CGU_PLL0USB_MDIV_MDEC_SHIFT	(0U)
#define CGU_PLL0USB_MDIV_MDEC_MASK 	(0x1FFFFU << CGU_PLL0USB_MDIV_MDEC_SHIFT)
#define CGU_PLL0USB_MDIV_MDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_MDIV_MDEC_SHIFT)) & CGU_PLL0USB_MDIV_MDEC_MASK)

/* CGU_PLL0USB_MDIV_SELP Bandwidth select P value
 * default: 11100
 */
#define CGU_PLL0USB_MDIV_SELP_SHIFT	(17U)
#define CGU_PLL0USB_MDIV_SELP_MASK 	(0x1FU << CGU_PLL0USB_MDIV_SELP_SHIFT)
#define CGU_PLL0USB_MDIV_SELP(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_MDIV_SELP_SHIFT)) & CGU_PLL0USB_MDIV_SELP_MASK)

/* CGU_PLL0USB_MDIV_SELI Bandwidth select I value
 * default: 010111
 */
#define CGU_PLL0USB_MDIV_SELI_SHIFT	(22U)
#define CGU_PLL0USB_MDIV_SELI_MASK 	(0x3FU << CGU_PLL0USB_MDIV_SELI_SHIFT)
#define CGU_PLL0USB_MDIV_SELI(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_MDIV_SELI_SHIFT)) & CGU_PLL0USB_MDIV_SELI_MASK)

/* CGU_PLL0USB_MDIV_SELR Bandwidth select R value
 * default: 0000
 */
#define CGU_PLL0USB_MDIV_SELR_SHIFT	(28U)
#define CGU_PLL0USB_MDIV_SELR_MASK 	(0x0FU << CGU_PLL0USB_MDIV_SELR_SHIFT)
#define CGU_PLL0USB_MDIV_SELR(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_MDIV_SELR_SHIFT)) & CGU_PLL0USB_MDIV_SELR_MASK)

/* PLL0USB_NP_DIV  at 0x40050028*/
/* CGU_PLL0USB_NP_DIV_PDEC Decoded P-divider coefficient value 000
 * default: 0010
 */
#define CGU_PLL0USB_NP_DIV_PDEC_SHIFT	(0U)
#define CGU_PLL0USB_NP_DIV_PDEC_MASK 	(0x7FU << CGU_PLL0USB_NP_DIV_PDEC_SHIFT)
#define CGU_PLL0USB_NP_DIV_PDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_NP_DIV_PDEC_SHIFT)) & CGU_PLL0USB_NP_DIV_PDEC_MASK)

/* CGU_PLL0USB_NP_DIV_NDEC Decoded N-divider coefficient value 1011
 * default: 0001
 */
#define CGU_PLL0USB_NP_DIV_NDEC_SHIFT	(12U)
#define CGU_PLL0USB_NP_DIV_NDEC_MASK 	(0x3FFU << CGU_PLL0USB_NP_DIV_NDEC_SHIFT)
#define CGU_PLL0USB_NP_DIV_NDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0USB_NP_DIV_NDEC_SHIFT)) & CGU_PLL0USB_NP_DIV_NDEC_MASK)

/* PLL0AUDIO_STAT  at 0x4005002C*/
/* CGU_PLL0AUDIO_STAT_LOCK PLL0 lock indicator
 * default: 0
 */
#define CGU_PLL0AUDIO_STAT_LOCK_SHIFT	(0U)
#define CGU_PLL0AUDIO_STAT_LOCK_MASK 	(0x01U << CGU_PLL0AUDIO_STAT_LOCK_SHIFT)
#define CGU_PLL0AUDIO_STAT_LOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_STAT_LOCK_SHIFT)) & CGU_PLL0AUDIO_STAT_LOCK_MASK)

/* CGU_PLL0AUDIO_STAT_FR PLL0 free running indicator
 * default: 0
 */
#define CGU_PLL0AUDIO_STAT_FR_SHIFT	(1U)
#define CGU_PLL0AUDIO_STAT_FR_MASK 	(0x01U << CGU_PLL0AUDIO_STAT_FR_SHIFT)
#define CGU_PLL0AUDIO_STAT_FR(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_STAT_FR_SHIFT)) & CGU_PLL0AUDIO_STAT_FR_MASK)

/* PLL0AUDIO_CTRL  at 0x40050030*/
/* CGU_PLL0AUDIO_CTRL_PD PLL0 power down
 * default: 1
 * 	 0x00 PLL0 enabled
 * 	 0x01 PLL0 powered down
 */
#define CGU_PLL0AUDIO_CTRL_PD_SHIFT	(0U)
#define CGU_PLL0AUDIO_CTRL_PD_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_PD_SHIFT)
#define CGU_PLL0AUDIO_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_PD_SHIFT)) & CGU_PLL0AUDIO_CTRL_PD_MASK)

/* CGU_PLL0AUDIO_CTRL_BYPASS Input clock bypass control
 * default: 1
 * 	 0x00 CCO clock sent to post-dividers. Use this in normal operation. (default).
 */
#define CGU_PLL0AUDIO_CTRL_BYPASS_SHIFT	(1U)
#define CGU_PLL0AUDIO_CTRL_BYPASS_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_BYPASS_SHIFT)
#define CGU_PLL0AUDIO_CTRL_BYPASS(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_BYPASS_SHIFT)) & CGU_PLL0AUDIO_CTRL_BYPASS_MASK)

/* CGU_PLL0AUDIO_CTRL_DIRECTI PLL0 direct input
 * default: 0
 */
#define CGU_PLL0AUDIO_CTRL_DIRECTI_SHIFT	(2U)
#define CGU_PLL0AUDIO_CTRL_DIRECTI_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_DIRECTI_SHIFT)
#define CGU_PLL0AUDIO_CTRL_DIRECTI(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_DIRECTI_SHIFT)) & CGU_PLL0AUDIO_CTRL_DIRECTI_MASK)

/* CGU_PLL0AUDIO_CTRL_DIRECTO PLL0 direct output
 * default: 0
 */
#define CGU_PLL0AUDIO_CTRL_DIRECTO_SHIFT	(3U)
#define CGU_PLL0AUDIO_CTRL_DIRECTO_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_DIRECTO_SHIFT)
#define CGU_PLL0AUDIO_CTRL_DIRECTO(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_DIRECTO_SHIFT)) & CGU_PLL0AUDIO_CTRL_DIRECTO_MASK)

/* CGU_PLL0AUDIO_CTRL_CLKEN PLL0 clock enable
 * default: 0
 */
#define CGU_PLL0AUDIO_CTRL_CLKEN_SHIFT	(4U)
#define CGU_PLL0AUDIO_CTRL_CLKEN_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_CLKEN_SHIFT)
#define CGU_PLL0AUDIO_CTRL_CLKEN(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_CLKEN_SHIFT)) & CGU_PLL0AUDIO_CTRL_CLKEN_MASK)

/* CGU_PLL0AUDIO_CTRL_FRM Free running mode
 * default: 0
 */
#define CGU_PLL0AUDIO_CTRL_FRM_SHIFT	(6U)
#define CGU_PLL0AUDIO_CTRL_FRM_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_FRM_SHIFT)
#define CGU_PLL0AUDIO_CTRL_FRM(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_FRM_SHIFT)) & CGU_PLL0AUDIO_CTRL_FRM_MASK)

/* CGU_PLL0AUDIO_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_PLL0AUDIO_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_PLL0AUDIO_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_AUTOBLOCK_SHIFT)
#define CGU_PLL0AUDIO_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_AUTOBLOCK_SHIFT)) & CGU_PLL0AUDIO_CTRL_AUTOBLOCK_MASK)

/* CGU_PLL0AUDIO_CTRL_SEL_EXT Select fractional divider.
 * default: 0
 * 	 0x00 Enable fractional divider.
 * 	 0x01 MDEC enabled. Fractional divider not used.
 */
#define CGU_PLL0AUDIO_CTRL_SEL_EXT_SHIFT	(13U)
#define CGU_PLL0AUDIO_CTRL_SEL_EXT_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_SEL_EXT_SHIFT)
#define CGU_PLL0AUDIO_CTRL_SEL_EXT(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_SEL_EXT_SHIFT)) & CGU_PLL0AUDIO_CTRL_SEL_EXT_MASK)

/* CGU_PLL0AUDIO_CTRL_MOD_PD Sigma-Delta modulator power-down
 * default: 1
 * 	 0x00 Sigma-Delta modulator enabled
 * 	 0x01 Sigma-Delta modulator powered down
 */
#define CGU_PLL0AUDIO_CTRL_MOD_PD_SHIFT	(14U)
#define CGU_PLL0AUDIO_CTRL_MOD_PD_MASK 	(0x01U << CGU_PLL0AUDIO_CTRL_MOD_PD_SHIFT)
#define CGU_PLL0AUDIO_CTRL_MOD_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_MOD_PD_SHIFT)) & CGU_PLL0AUDIO_CTRL_MOD_PD_MASK)

/* CGU_PLL0AUDIO_CTRL_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_PLL0AUDIO_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_PLL0AUDIO_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_PLL0AUDIO_CTRL_CLK_SEL_SHIFT)
#define CGU_PLL0AUDIO_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_CTRL_CLK_SEL_SHIFT)) & CGU_PLL0AUDIO_CTRL_CLK_SEL_MASK)

/* PLL0AUDIO_MDIV  at 0x40050034*/
/* CGU_PLL0AUDIO_MDIV_MDEC Decoded M-divider coefficient value. Select values for the M-divider between 1 and 131071.
 * default: 0x5B6A
 */
#define CGU_PLL0AUDIO_MDIV_MDEC_SHIFT	(0U)
#define CGU_PLL0AUDIO_MDIV_MDEC_MASK 	(0x1FFFFU << CGU_PLL0AUDIO_MDIV_MDEC_SHIFT)
#define CGU_PLL0AUDIO_MDIV_MDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_MDIV_MDEC_SHIFT)) & CGU_PLL0AUDIO_MDIV_MDEC_MASK)

/* PLL0AUDIO_NP_DIV  at 0x40050038*/
/* CGU_PLL0AUDIO_NP_DIV_PDEC Decoded P-divider coefficient value 000
 * default: 0010
 */
#define CGU_PLL0AUDIO_NP_DIV_PDEC_SHIFT	(0U)
#define CGU_PLL0AUDIO_NP_DIV_PDEC_MASK 	(0x7FU << CGU_PLL0AUDIO_NP_DIV_PDEC_SHIFT)
#define CGU_PLL0AUDIO_NP_DIV_PDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_NP_DIV_PDEC_SHIFT)) & CGU_PLL0AUDIO_NP_DIV_PDEC_MASK)

/* CGU_PLL0AUDIO_NP_DIV_NDEC Decoded N-divider coefficient value 1011
 * default: 0001
 */
#define CGU_PLL0AUDIO_NP_DIV_NDEC_SHIFT	(12U)
#define CGU_PLL0AUDIO_NP_DIV_NDEC_MASK 	(0x3FFU << CGU_PLL0AUDIO_NP_DIV_NDEC_SHIFT)
#define CGU_PLL0AUDIO_NP_DIV_NDEC(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_NP_DIV_NDEC_SHIFT)) & CGU_PLL0AUDIO_NP_DIV_NDEC_MASK)

/* PLL0AUDIO_FRAC  at 0x4005003C*/
/* CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL PLL fractional divider control word 000
 * default: 0000
 */
#define CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL_SHIFT	(0U)
#define CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL_MASK 	(0x3FFFFFU << CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL_SHIFT)
#define CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL_SHIFT)) & CGU_PLL0AUDIO_FRAC_PLLFRACT_CTRL_MASK)

/* PLL1_STAT  at 0x40050040*/
/* CGU_PLL1_STAT_LOCK PLL1 lock indicator
 * default: 0
 */
#define CGU_PLL1_STAT_LOCK_SHIFT	(0U)
#define CGU_PLL1_STAT_LOCK_MASK 	(0x01U << CGU_PLL1_STAT_LOCK_SHIFT)
#define CGU_PLL1_STAT_LOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_STAT_LOCK_SHIFT)) & CGU_PLL1_STAT_LOCK_MASK)

/* PLL1_CTRL  at 0x40050044*/
/* CGU_PLL1_CTRL_PD PLL1 power down
 * default: 1
 * 	 0x00 PLL1 enabled
 * 	 0x01 PLL1 powered down
 */
#define CGU_PLL1_CTRL_PD_SHIFT	(0U)
#define CGU_PLL1_CTRL_PD_MASK 	(0x01U << CGU_PLL1_CTRL_PD_SHIFT)
#define CGU_PLL1_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_PD_SHIFT)) & CGU_PLL1_CTRL_PD_MASK)

/* CGU_PLL1_CTRL_BYPASS Input clock bypass control
 * default: 1
 * 	 0x00 CCO clock sent to post-dividers. Use for normal operation.
 */
#define CGU_PLL1_CTRL_BYPASS_SHIFT	(1U)
#define CGU_PLL1_CTRL_BYPASS_MASK 	(0x01U << CGU_PLL1_CTRL_BYPASS_SHIFT)
#define CGU_PLL1_CTRL_BYPASS(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_BYPASS_SHIFT)) & CGU_PLL1_CTRL_BYPASS_MASK)

/* CGU_PLL1_CTRL_FBSEL PLL feedback select (see Figure 29 “PLL1 block diagram”).
 * default: 0
 * 	 0x00 CCO output is used as feedback divider input clock.
 * 	 0x01 PLL output clock (clkout) is used as feedback divider input clock. Use for normal operation
 */
#define CGU_PLL1_CTRL_FBSEL_SHIFT	(6U)
#define CGU_PLL1_CTRL_FBSEL_MASK 	(0x01U << CGU_PLL1_CTRL_FBSEL_SHIFT)
#define CGU_PLL1_CTRL_FBSEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_FBSEL_SHIFT)) & CGU_PLL1_CTRL_FBSEL_MASK)

/* CGU_PLL1_CTRL_DIRECT PLL direct CCO output
 * default: 0
 * 	 0x00 Disabled
 * 	 0x01 Enabled
 */
#define CGU_PLL1_CTRL_DIRECT_SHIFT	(7U)
#define CGU_PLL1_CTRL_DIRECT_MASK 	(0x01U << CGU_PLL1_CTRL_DIRECT_SHIFT)
#define CGU_PLL1_CTRL_DIRECT(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_DIRECT_SHIFT)) & CGU_PLL1_CTRL_DIRECT_MASK)

/* CGU_PLL1_CTRL_PSEL Post-divider division ratio P. The value applied is 2xP.
 * default: 01
 * 	 0x00 1
 * 	 0x01 2 (default)
 * 	 0x02 4
 * 	 0x03 8
 */
#define CGU_PLL1_CTRL_PSEL_SHIFT	(8U)
#define CGU_PLL1_CTRL_PSEL_MASK 	(0x03U << CGU_PLL1_CTRL_PSEL_SHIFT)
#define CGU_PLL1_CTRL_PSEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_PSEL_SHIFT)) & CGU_PLL1_CTRL_PSEL_MASK)

/* CGU_PLL1_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_PLL1_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_PLL1_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_PLL1_CTRL_AUTOBLOCK_SHIFT)
#define CGU_PLL1_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_AUTOBLOCK_SHIFT)) & CGU_PLL1_CTRL_AUTOBLOCK_MASK)

/* CGU_PLL1_CTRL_NSEL Pre-divider division ratio N
 * default: 10
 * 	 0x00 1
 * 	 0x01 2
 * 	 0x02 3 (default)
 * 	 0x03 4
 */
#define CGU_PLL1_CTRL_NSEL_SHIFT	(12U)
#define CGU_PLL1_CTRL_NSEL_MASK 	(0x03U << CGU_PLL1_CTRL_NSEL_SHIFT)
#define CGU_PLL1_CTRL_NSEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_NSEL_SHIFT)) & CGU_PLL1_CTRL_NSEL_MASK)

/* CGU_PLL1_CTRL_MSEL Feedback-divider division ratio (M) 00000000 = 1 00000001 = 2 ... 11111111 = 256
 * default: 11000
 */
#define CGU_PLL1_CTRL_MSEL_SHIFT	(16U)
#define CGU_PLL1_CTRL_MSEL_MASK 	(0xFFU << CGU_PLL1_CTRL_MSEL_SHIFT)
#define CGU_PLL1_CTRL_MSEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_MSEL_SHIFT)) & CGU_PLL1_CTRL_MSEL_MASK)

/* CGU_PLL1_CTRL_CLK_SEL Clock-source selection.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_PLL1_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_PLL1_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_PLL1_CTRL_CLK_SEL_SHIFT)
#define CGU_PLL1_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_PLL1_CTRL_CLK_SEL_SHIFT)) & CGU_PLL1_CTRL_CLK_SEL_MASK)

/* IDIVA_CTRL  at 0x40050048*/
/* CGU_IDIVA_CTRL_PD Integer divider A power down
 * default: 0
 * 	 0x00 IDIVA enabled (default)
 * 	 0x01 power-down
 */
#define CGU_IDIVA_CTRL_PD_SHIFT	(0U)
#define CGU_IDIVA_CTRL_PD_MASK 	(0x01U << CGU_IDIVA_CTRL_PD_SHIFT)
#define CGU_IDIVA_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVA_CTRL_PD_SHIFT)) & CGU_IDIVA_CTRL_PD_MASK)

/* CGU_IDIVA_CTRL_IDIV Integer divider A divider values (1/(IDIV + 1))
 * default: 00
 * 	 0x00 1 (default)
 * 	 0x01 2
 * 	 0x02 3
 * 	 0x03 4
 */
#define CGU_IDIVA_CTRL_IDIV_SHIFT	(2U)
#define CGU_IDIVA_CTRL_IDIV_MASK 	(0x03U << CGU_IDIVA_CTRL_IDIV_SHIFT)
#define CGU_IDIVA_CTRL_IDIV(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVA_CTRL_IDIV_SHIFT)) & CGU_IDIVA_CTRL_IDIV_MASK)

/* CGU_IDIVA_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_IDIVA_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_IDIVA_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_IDIVA_CTRL_AUTOBLOCK_SHIFT)
#define CGU_IDIVA_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVA_CTRL_AUTOBLOCK_SHIFT)) & CGU_IDIVA_CTRL_AUTOBLOCK_MASK)

/* CGU_IDIVA_CTRL_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 */
#define CGU_IDIVA_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_IDIVA_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_IDIVA_CTRL_CLK_SEL_SHIFT)
#define CGU_IDIVA_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVA_CTRL_CLK_SEL_SHIFT)) & CGU_IDIVA_CTRL_CLK_SEL_MASK)

/* IDIVB_C_D_CTRL  at 0x4005004C;IDIVC_CTRL,address0x40050050;IDIVC_CTRL,address0x40050054*/
/* CGU_IDIVB_C_D_CTRL_PD Integer divider power down
 * default: 0
 * 	 0x00 IDIV enabled (default)
 * 	 0x01 power-down
 */
#define CGU_IDIVB_C_D_CTRL_PD_SHIFT	(0U)
#define CGU_IDIVB_C_D_CTRL_PD_MASK 	(0x01U << CGU_IDIVB_C_D_CTRL_PD_SHIFT)
#define CGU_IDIVB_C_D_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVB_C_D_CTRL_PD_SHIFT)) & CGU_IDIVB_C_D_CTRL_PD_MASK)

/* CGU_IDIVB_C_D_CTRL_IDIV Integer divider B, C, D divider values (1/(IDIV + 1)) 0000 = 1 (default) 0001 = 2 ... 1111 = 16
 * default: 0000
 */
#define CGU_IDIVB_C_D_CTRL_IDIV_SHIFT	(2U)
#define CGU_IDIVB_C_D_CTRL_IDIV_MASK 	(0x0FU << CGU_IDIVB_C_D_CTRL_IDIV_SHIFT)
#define CGU_IDIVB_C_D_CTRL_IDIV(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVB_C_D_CTRL_IDIV_SHIFT)) & CGU_IDIVB_C_D_CTRL_IDIV_MASK)

/* CGU_IDIVB_C_D_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_IDIVB_C_D_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_IDIVB_C_D_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_IDIVB_C_D_CTRL_AUTOBLOCK_SHIFT)
#define CGU_IDIVB_C_D_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVB_C_D_CTRL_AUTOBLOCK_SHIFT)) & CGU_IDIVB_C_D_CTRL_AUTOBLOCK_MASK)

/* CGU_IDIVB_C_D_CTRL_CLK_SEL Clock-source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 */
#define CGU_IDIVB_C_D_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_IDIVB_C_D_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_IDIVB_C_D_CTRL_CLK_SEL_SHIFT)
#define CGU_IDIVB_C_D_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVB_C_D_CTRL_CLK_SEL_SHIFT)) & CGU_IDIVB_C_D_CTRL_CLK_SEL_MASK)

/* IDIVE_CTRL  at 0x40050058*/
/* CGU_IDIVE_CTRL_PD Integer divider power down
 * default: 0
 * 	 0x00 IDIV enabled (default)
 * 	 0x01 power-down
 */
#define CGU_IDIVE_CTRL_PD_SHIFT	(0U)
#define CGU_IDIVE_CTRL_PD_MASK 	(0x01U << CGU_IDIVE_CTRL_PD_SHIFT)
#define CGU_IDIVE_CTRL_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVE_CTRL_PD_SHIFT)) & CGU_IDIVE_CTRL_PD_MASK)

/* CGU_IDIVE_CTRL_IDIV Integer divider E divider values (1/(IDIV + 1)) 00000000 = 1 (default) 00000001 = 2 ... 111111111 = 256 000000
 * default: 00
 */
#define CGU_IDIVE_CTRL_IDIV_SHIFT	(2U)
#define CGU_IDIVE_CTRL_IDIV_MASK 	(0xFFU << CGU_IDIVE_CTRL_IDIV_SHIFT)
#define CGU_IDIVE_CTRL_IDIV(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVE_CTRL_IDIV_SHIFT)) & CGU_IDIVE_CTRL_IDIV_MASK)

/* CGU_IDIVE_CTRL_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_IDIVE_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_IDIVE_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_IDIVE_CTRL_AUTOBLOCK_SHIFT)
#define CGU_IDIVE_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVE_CTRL_AUTOBLOCK_SHIFT)) & CGU_IDIVE_CTRL_AUTOBLOCK_MASK)

/* CGU_IDIVE_CTRL_CLK_SEL Clock-source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 */
#define CGU_IDIVE_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_IDIVE_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_IDIVE_CTRL_CLK_SEL_SHIFT)
#define CGU_IDIVE_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_IDIVE_CTRL_CLK_SEL_SHIFT)) & CGU_IDIVE_CTRL_CLK_SEL_MASK)

/* BASE_SAFE_CLK  at 0x4005005C*/
/* CGU_BASE_SAFE_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_SAFE_CLK_PD_SHIFT	(0U)
#define CGU_BASE_SAFE_CLK_PD_MASK 	(0x01U << CGU_BASE_SAFE_CLK_PD_SHIFT)
#define CGU_BASE_SAFE_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_SAFE_CLK_PD_SHIFT)) & CGU_BASE_SAFE_CLK_PD_MASK)

/* CGU_BASE_SAFE_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_SAFE_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_SAFE_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_SAFE_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_SAFE_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_SAFE_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_SAFE_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_SAFE_CLK_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x01 IRC (default)
 */
#define CGU_BASE_SAFE_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_SAFE_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_SAFE_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_SAFE_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_SAFE_CLK_CLK_SEL_SHIFT)) & CGU_BASE_SAFE_CLK_CLK_SEL_MASK)

/* BASE_USB0_CLK  at 0x40050060*/
/* CGU_BASE_USB0_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_USB0_CLK_PD_SHIFT	(0U)
#define CGU_BASE_USB0_CLK_PD_MASK 	(0x01U << CGU_BASE_USB0_CLK_PD_SHIFT)
#define CGU_BASE_USB0_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB0_CLK_PD_SHIFT)) & CGU_BASE_USB0_CLK_PD_MASK)

/* CGU_BASE_USB0_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_USB0_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_USB0_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_USB0_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_USB0_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB0_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_USB0_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_USB0_CLK_CLK_SEL Clock-source selection.
 * default: 0x07
 * 	 0x07 PLL0USB (default)
 */
#define CGU_BASE_USB0_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_USB0_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_USB0_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_USB0_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB0_CLK_CLK_SEL_SHIFT)) & CGU_BASE_USB0_CLK_CLK_SEL_MASK)

/* BASE_PERIPH_CLK  at 0x40050064*/
/* CGU_BASE_PERIPH_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_PERIPH_CLK_PD_SHIFT	(0U)
#define CGU_BASE_PERIPH_CLK_PD_MASK 	(0x01U << CGU_BASE_PERIPH_CLK_PD_SHIFT)
#define CGU_BASE_PERIPH_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_PERIPH_CLK_PD_SHIFT)) & CGU_BASE_PERIPH_CLK_PD_MASK)

/* CGU_BASE_PERIPH_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_PERIPH_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_PERIPH_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_PERIPH_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_PERIPH_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_PERIPH_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_PERIPH_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_PERIPH_CLK_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_PERIPH_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_PERIPH_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_PERIPH_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_PERIPH_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_PERIPH_CLK_CLK_SEL_SHIFT)) & CGU_BASE_PERIPH_CLK_CLK_SEL_MASK)

/* BASE_USB1_CLK  at 0x40050068*/
/* CGU_BASE_USB1_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_USB1_CLK_PD_SHIFT	(0U)
#define CGU_BASE_USB1_CLK_PD_MASK 	(0x01U << CGU_BASE_USB1_CLK_PD_SHIFT)
#define CGU_BASE_USB1_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB1_CLK_PD_SHIFT)) & CGU_BASE_USB1_CLK_PD_MASK)

/* CGU_BASE_USB1_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_USB1_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_USB1_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_USB1_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_USB1_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB1_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_USB1_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_USB1_CLK_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_USB1_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_USB1_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_USB1_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_USB1_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_USB1_CLK_CLK_SEL_SHIFT)) & CGU_BASE_USB1_CLK_CLK_SEL_MASK)

/* BASE_M4_UART3_CLK  at 0x4005006Cto0x400500A8*/
/* CGU_BASE_M4_UART3_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_M4_UART3_CLK_PD_SHIFT	(0U)
#define CGU_BASE_M4_UART3_CLK_PD_MASK 	(0x01U << CGU_BASE_M4_UART3_CLK_PD_SHIFT)
#define CGU_BASE_M4_UART3_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_M4_UART3_CLK_PD_SHIFT)) & CGU_BASE_M4_UART3_CLK_PD_MASK)

/* CGU_BASE_M4_UART3_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_M4_UART3_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_M4_UART3_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_M4_UART3_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_M4_UART3_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_M4_UART3_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_M4_UART3_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_M4_UART3_CLK_CLK_SEL Clock source selection. All other values are reserved.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_M4_UART3_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_M4_UART3_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_M4_UART3_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_M4_UART3_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_M4_UART3_CLK_CLK_SEL_SHIFT)) & CGU_BASE_M4_UART3_CLK_CLK_SEL_MASK)

/* BASE_OUT_CLK  at s0x400500AC*/
/* CGU_BASE_OUT_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_OUT_CLK_PD_SHIFT	(0U)
#define CGU_BASE_OUT_CLK_PD_MASK 	(0x01U << CGU_BASE_OUT_CLK_PD_SHIFT)
#define CGU_BASE_OUT_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_OUT_CLK_PD_SHIFT)) & CGU_BASE_OUT_CLK_PD_MASK)

/* CGU_BASE_OUT_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_OUT_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_OUT_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_OUT_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_OUT_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_OUT_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_OUT_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_OUT_CLK_CLK_SEL Clock-source selection.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_OUT_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_OUT_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_OUT_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_OUT_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_OUT_CLK_CLK_SEL_SHIFT)) & CGU_BASE_OUT_CLK_CLK_SEL_MASK)

/* BASE_APLL_CLK  at s0x400500C0*/
/* CGU_BASE_APLL_CLK_PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_APLL_CLK_PD_SHIFT	(0U)
#define CGU_BASE_APLL_CLK_PD_MASK 	(0x01U << CGU_BASE_APLL_CLK_PD_SHIFT)
#define CGU_BASE_APLL_CLK_PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_APLL_CLK_PD_SHIFT)) & CGU_BASE_APLL_CLK_PD_MASK)

/* CGU_BASE_APLL_CLK_AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_APLL_CLK_AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_APLL_CLK_AUTOBLOCK_MASK 	(0x01U << CGU_BASE_APLL_CLK_AUTOBLOCK_SHIFT)
#define CGU_BASE_APLL_CLK_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_APLL_CLK_AUTOBLOCK_SHIFT)) & CGU_BASE_APLL_CLK_AUTOBLOCK_MASK)

/* CGU_BASE_APLL_CLK_CLK_SEL Clock-source selection.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_APLL_CLK_CLK_SEL_SHIFT	(24U)
#define CGU_BASE_APLL_CLK_CLK_SEL_MASK 	(0x1FU << CGU_BASE_APLL_CLK_CLK_SEL_SHIFT)
#define CGU_BASE_APLL_CLK_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_APLL_CLK_CLK_SEL_SHIFT)) & CGU_BASE_APLL_CLK_CLK_SEL_MASK)

/* BASE_CGU_OUT0_OUT1_CLK   at s0x400500C4to0x400500C8*/
/* CGU_BASE_CGU_OUT0_OUT1_CLK _PD Output stage power down
 * default: 0
 * 	 0x00 Output stage enabled (default)
 * 	 0x01 power-down
 */
#define CGU_BASE_CGU_OUT0_OUT1_CLK _PD_SHIFT	(0U)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _PD_MASK 	(0x01U << CGU_BASE_CGU_OUT0_OUT1_CLK _PD_SHIFT)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _PD(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_CGU_OUT0_OUT1_CLK _PD_SHIFT)) & CGU_BASE_CGU_OUT0_OUT1_CLK _PD_MASK)

/* CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK Block clock automatically during frequency change
 * default: 0
 * 	 0x00 Autoblocking disabled
 * 	 0x01 Autoblocking enabled
 */
#define CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK_SHIFT	(11U)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK_MASK 	(0x01U << CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK_SHIFT)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK_SHIFT)) & CGU_BASE_CGU_OUT0_OUT1_CLK _AUTOBLOCK_MASK)

/* CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL Clock-source selection.
 * default: 0x01
 * 	 0x00 32 kHz oscillator
 * 	 0x01 IRC (default)
 * 	 0x02 ENET_RX_CLK
 * 	 0x03 ENET_TX_CLK
 * 	 0x04 GP_CLKIN
 * 	 0x06 Crystal oscillator
 * 	 0x07 PLL0USB
 * 	 0x08 PLL0AUDIO
 * 	 0x09 PLL1
 * 	 0x0C IDIVA
 * 	 0x0D IDIVB
 * 	 0x0E IDIVC
 * 	 0x0F IDIVD
 * 	 0x10 IDIVE
 */
#define CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL_SHIFT	(24U)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL_MASK 	(0x1FU << CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL_SHIFT)
#define CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL_SHIFT)) & CGU_BASE_CGU_OUT0_OUT1_CLK _CLK_SEL_MASK)


#define CGU_CTRL_ENABLE_SHIFT	(0U)
#define CGU_CTRL_ENABLE_MASK 	(0x01U << CGU_CTRL_ENABLE_SHIFT)
#define CGU_CTRL_ENABLE(x)		(((uint32_t)(((uint32_t)(x)) << CGU_CTRL_ENABLE_SHIFT)) & CGU_CTRL_ENABLE_MASK)

#define CGU_CTRL_CLK_SEL_SHIFT	(24U)
#define CGU_CTRL_CLK_SEL_MASK 	(0x1FU << CGU_CTRL_CLK_SEL_SHIFT)
#define CGU_CTRL_CLK_SEL(x)		(((uint32_t)(((uint32_t)(x)) << CGU_CTRL_CLK_SEL_SHIFT)) & CGU_CTRL_CLK_SEL_MASK)

#define CGU_CTRL_AUTOBLOCK_SHIFT	(11U)
#define CGU_CTRL_AUTOBLOCK_MASK 	(0x01U << CGU_CTRL_AUTOBLOCK_SHIFT)
#define CGU_CTRL_AUTOBLOCK(x)		(((uint32_t)(((uint32_t)(x)) << CGU_CTRL_AUTOBLOCK_SHIFT)) & CGU_CTRL_AUTOBLOCK_MASK)

/* CCU1_PM at 0x40051000 and CCU2_PM at 0x40052000
 * default: 0
 * 	 0x00 Normal operation.
 * 	 0x01 Clocks with wake-up mode enabled (W = 1) are disabled.
 */
#define CCU_CCU1_2_PM_PD_SHIFT	(0U)
#define CCU_CCU1_2_PM_PD_MASK 	(0x01U << CCU_CCU1_2_PM_PD_SHIFT)
#define CCU_CCU1_2_PM_PD(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CCU1_2_PM_PD_SHIFT)) & CCU_CCU1_2_PM_PD_MASK)

/* CLK_XXX_CFG  at s0x40051100,0x40051104,...,0x40051A00*/
/* CCU_CLK_XXX_CFG_RUN Run enable
 * default: 1
 * 	 0x00 Clock is disabled.
 * 	 0x01 Clock is enabled.
 */
#define CCU_CLK_XXX_CFG_RUN_SHIFT	(0U)
#define CCU_CLK_XXX_CFG_RUN_MASK 	(0x01U << CCU_CLK_XXX_CFG_RUN_SHIFT)
#define CCU_CLK_XXX_CFG_RUN(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_CFG_RUN_SHIFT)) & CCU_CLK_XXX_CFG_RUN_MASK)

/* CCU_CLK_XXX_CFG_AUTO Auto (AHB disable mechanism) enable
 * default: 0
 * 	 0x00 Auto is disabled.
 * 	 0x01 Auto is enabled.
 */
#define CCU_CLK_XXX_CFG_AUTO_SHIFT	(1U)
#define CCU_CLK_XXX_CFG_AUTO_MASK 	(0x01U << CCU_CLK_XXX_CFG_AUTO_SHIFT)
#define CCU_CLK_XXX_CFG_AUTO(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_CFG_AUTO_SHIFT)) & CCU_CLK_XXX_CFG_AUTO_MASK)

/* CCU_CLK_XXX_CFG_WAKEUP Wake-up mechanism enable
 * default: 0
 * 	 0x00 Wake-up is disabled.
 * 	 0x01 Wake-up is enabled.
 */
#define CCU_CLK_XXX_CFG_WAKEUP_SHIFT	(2U)
#define CCU_CLK_XXX_CFG_WAKEUP_MASK 	(0x01U << CCU_CLK_XXX_CFG_WAKEUP_SHIFT)
#define CCU_CLK_XXX_CFG_WAKEUP(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_CFG_WAKEUP_SHIFT)) & CCU_CLK_XXX_CFG_WAKEUP_MASK)

/* CLK_EMCDIV_CFG  at s0x40051478*/
/* CCU_CLK_EMCDIV_CFG_RUN Run enable
 * default: 1
 * 	 0x00 Clock is disabled.
 * 	 0x01 Clock is enabled.
 */
#define CCU_CLK_EMCDIV_CFG_RUN_SHIFT	(0U)
#define CCU_CLK_EMCDIV_CFG_RUN_MASK 	(0x01U << CCU_CLK_EMCDIV_CFG_RUN_SHIFT)
#define CCU_CLK_EMCDIV_CFG_RUN(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_EMCDIV_CFG_RUN_SHIFT)) & CCU_CLK_EMCDIV_CFG_RUN_MASK)

/* CCU_CLK_EMCDIV_CFG_AUTO Auto (AHB disable mechanism) enable
 * default: 0
 * 	 0x00 Auto is disabled.
 * 	 0x01 Auto is enabled.
 */
#define CCU_CLK_EMCDIV_CFG_AUTO_SHIFT	(1U)
#define CCU_CLK_EMCDIV_CFG_AUTO_MASK 	(0x01U << CCU_CLK_EMCDIV_CFG_AUTO_SHIFT)
#define CCU_CLK_EMCDIV_CFG_AUTO(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_EMCDIV_CFG_AUTO_SHIFT)) & CCU_CLK_EMCDIV_CFG_AUTO_MASK)

/* CCU_CLK_EMCDIV_CFG_WAKEUP Wake-up mechanism enable
 * default: 0
 * 	 0x00 Wake-up is disabled.
 * 	 0x01 Wake-up is enabled.
 */
#define CCU_CLK_EMCDIV_CFG_WAKEUP_SHIFT	(2U)
#define CCU_CLK_EMCDIV_CFG_WAKEUP_MASK 	(0x01U << CCU_CLK_EMCDIV_CFG_WAKEUP_SHIFT)
#define CCU_CLK_EMCDIV_CFG_WAKEUP(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_EMCDIV_CFG_WAKEUP_SHIFT)) & CCU_CLK_EMCDIV_CFG_WAKEUP_MASK)

/* CCU_CLK_EMCDIV_CFG_DIV Clock divider value
 * default: 0
 * 	 0x00 No division (divide by 1).
 * 	 0x01 Divide by 2.
 */
#define CCU_CLK_EMCDIV_CFG_DIV_SHIFT	(5U)
#define CCU_CLK_EMCDIV_CFG_DIV_MASK 	(0x07U << CCU_CLK_EMCDIV_CFG_DIV_SHIFT)
#define CCU_CLK_EMCDIV_CFG_DIV(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_EMCDIV_CFG_DIV_SHIFT)) & CCU_CLK_EMCDIV_CFG_DIV_MASK)

/* CLK_XXX_STAT  at s0x40051104,0x4005110C,...,0x40051A04*/
/* CCU_CLK_XXX_STAT_RUN Run
 * default: enable
 * 	 0x00 = clock is disabled.
 * 	 0x01 = clock is enabled.
 */
#define CCU_CLK_XXX_STAT_RUN_SHIFT	(0U)
#define CCU_CLK_XXX_STAT_RUN_MASK 	(0x01U << CCU_CLK_XXX_STAT_RUN_SHIFT)
#define CCU_CLK_XXX_STAT_RUN(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_STAT_RUN_SHIFT)) & CCU_CLK_XXX_STAT_RUN_MASK)

/* CCU_CLK_XXX_STAT_AUTO Auto (AHB disable mechanism)
 * default: enable
 * 	 0x00 = Auto is disabled.
 * 	 0x01 = Auto is enabled.
 */
#define CCU_CLK_XXX_STAT_AUTO_SHIFT	(1U)
#define CCU_CLK_XXX_STAT_AUTO_MASK 	(0x01U << CCU_CLK_XXX_STAT_AUTO_SHIFT)
#define CCU_CLK_XXX_STAT_AUTO(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_STAT_AUTO_SHIFT)) & CCU_CLK_XXX_STAT_AUTO_MASK)

/* CCU_CLK_XXX_STAT_WAKEUP Wake-up mechanism
 * default: enable
 * 	 0x00 = Wake-up is disabled.
 * 	 0x01 = Wake-up is enabled.
 */
#define CCU_CLK_XXX_STAT_WAKEUP_SHIFT	(2U)
#define CCU_CLK_XXX_STAT_WAKEUP_MASK 	(0x01U << CCU_CLK_XXX_STAT_WAKEUP_SHIFT)
#define CCU_CLK_XXX_STAT_WAKEUP(x)		(((uint32_t)(((uint32_t)(x)) << CCU_CLK_XXX_STAT_WAKEUP_SHIFT)) & CCU_CLK_XXX_STAT_WAKEUP_MASK)

/** @ingroup CLOCK_18XX_43XX
 * @{
 */

/**
 * Audio or USB PLL selection
 */
typedef enum CHIP_CGU_USB_AUDIO_PLL {
	CGU_USB_PLL,
	CGU_AUDIO_PLL
} CHIP_CGU_USB_AUDIO_PLL_T;

/**
 * PLL register block
 */
typedef struct {
	__I  uint32_t  PLL_STAT;				/*!< PLL status register */
	__IO uint32_t  PLL_CTRL;				/*!< PLL control register */
	__IO uint32_t  PLL_MDIV;				/*!< PLL M-divider register */
	__IO uint32_t  PLL_NP_DIV;				/*!< PLL N/P-divider register */
} CGU_PLL_REG_T;

/**
 * @brief LPC18XX/43XX CGU register block structure
 */
typedef struct {							/*!< (@ 0x40050000) CGU Structure          */
	__I  uint32_t  RESERVED0[5];
	__IO uint32_t  FREQ_MON;				/*!< (@ 0x40050014) Frequency monitor register */
	__IO uint32_t  XTAL_OSC_CTRL;			/*!< (@ 0x40050018) Crystal oscillator control register */
	CGU_PLL_REG_T  PLL[CGU_AUDIO_PLL + 1];	/*!< (@ 0x4005001C) USB and audio PLL blocks */
	__IO uint32_t  PLL0AUDIO_FRAC;			/*!< (@ 0x4005003C) PLL0 (audio)           */
	__I  uint32_t  PLL1_STAT;				/*!< (@ 0x40050040) PLL1 status register   */
	__IO uint32_t  PLL1_CTRL;				/*!< (@ 0x40050044) PLL1 control register  */
	__IO uint32_t  IDIV_CTRL[CLK_IDIV_LAST];/*!< (@ 0x40050048) Integer divider A-E control registers */
	__IO uint32_t  BASE_CLK[CLK_BASE_LAST];	/*!< (@ 0x4005005C) Start of base clock registers */
} LPC_CGU_T;

/**
 * @brief CCU clock config/status register pair
 */
typedef struct {
	__IO uint32_t  CFG;						/*!< CCU clock configuration register */
	__I  uint32_t  STAT;					/*!< CCU clock status register */
} CCU_CFGSTAT_T;

/**
 * @brief CCU1 register block structure
 */
typedef struct {							/*!< (@ 0x40051000) CCU1 Structure         */
	__IO uint32_t  PM;						/*!< (@ 0x40051000) CCU1 power mode register */
	__I  uint32_t  BASE_STAT;				/*!< (@ 0x40051004) CCU1 base clocks status register */
	__I  uint32_t  RESERVED0[62];
	CCU_CFGSTAT_T  CLKCCU[CLK_CCU1_LAST];	/*!< (@ 0x40051100) Start of CCU1 clock registers */
} LPC_CCU1_T;

/**
 * @brief CCU2 register block structure
 */
typedef struct {							/*!< (@ 0x40052000) CCU2 Structure         */
	__IO uint32_t  PM;						/*!< (@ 0x40052000) Power mode register    */
	__I  uint32_t  BASE_STAT;				/*!< (@ 0x40052004) CCU base clocks status register */
	__I  uint32_t  RESERVED0[62];
	CCU_CFGSTAT_T  CLKCCU[CLK_CCU2_LAST - CLK_CCU1_LAST];	/*!< (@ 0x40052100) Start of CCU2 clock registers */
} LPC_CCU2_T;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CGUCCU_18XX_43XX_H_ */






