/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT2_RMIISRC_EXTAL 0x00u /*!<@brief RMII clock source select: EXTAL clock */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
