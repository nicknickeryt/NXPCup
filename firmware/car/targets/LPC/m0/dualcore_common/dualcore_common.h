#ifndef __DUALCORE_COMMON_H_
#define __DUALCORE_COMMON_H_
#include <stdint.h>
 #include "board.h"

/*
 * The Stack sizes are currently used by Code Red LPCXpresso tool chain
 * only. In future, this will be used by Keil & IAR tool chains also
 */
#ifndef STACK_SIZE
#define STACK_SIZE  0x800
#endif

#ifndef HEAP_SIZE
#define HEAP_SIZE   0x6000
#endif

/* Priority of various IRQs used in dual-core examples */
/* lower the number higher the priority */
#define IRQ_PRIO_IPC              7

/*
 * Absolute addresses used by both cores. Make sure these memory regions are not
 * used in DATA/CODE regions of the generated image (check the map file to verify)
 * Currently the shared memory buffers are located at the end of the SRAM area
 * 0x1008000. Last 64 bytes of the 40K area are used for M4 queue header and
 * the 64 bytes above that is used by M0 queue. This value need not be changed
 * for normal circumstances.
 */
#ifndef SHARED_MEM_M0
#define SHARED_MEM_M0          0x10089F80
#endif
#ifndef SHARED_MEM_M4
#define SHARED_MEM_M4          0x10089FC0
#endif


#ifdef CORE_M0
/* Delay and LED to be blinked by M4 Core */
#define BLINK_LED                    0
#define BLINKY_DEFAULT_DELAY         500
#endif /* CORE_M0 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	IPC Example initialization
 *
 * This function initializes the IPC Example implementation. It calls
 * IPC_initMsgQueue() to initialize the IPC queue of the calling core,
 * by allocating and passing a queue array. It will be called by the
 * @link EXAMPLE_DUALCORE_CMN_MAIN main()@endlink of both core-M0 and
 * core-M4 code for all the dual core examples.
 *
 * @return	None
 */
extern void IPCEX_Init(void);

/**
 * @brief	Dual core Blinky example initialization
 *
 * This function will register the blink function to the event queue of
 * a Core (M0/M4). Whenever a blink event is received from the other core
 * the registered call-back function will be called-back by the ipcex_task(),
 * *This function is called by @link EXAMPLE_DUALCORE_CMN_MAIN main()@endlink
 * only when EXAMPLE_BLINKY is defined*. In all the dual core examples
 * EXAMPLE_BLINKY is defined by default.
 *
 * @return	None
 */
extern void BLINKY_Init(void);

/**
 * @brief	Dual Core IPC example implementation task
 *
 * This task receives the message from the other core and will call-back the
 * function (if registered via ipcex_register_callback()) corresponding to
 * the received message. This function is called by @link
 * EXAMPLE_DUALCORE_CMN_MAIN main()@endlink.
 *
 * @return	None
 */
extern void ipcex_tasks(void);

/**
 * @brief	Dual core blinky task
 *
 * Calling this function calls the blinky task in which will blink the
 * LEDs. When no OS is specified this will be repeatedly *called from
 * dual core @link EXAMPLE_DUALCORE_CMN_MAIN main()@endlink or once
 * if any OS is defined, only when EXAMPLE_BLINKY is defined*. In all
 * the dual core examples EXAMPLE_BLINKY is defined by default.
 *
 * @return	None
 */
extern void blinky_tasks(void);

/**
 * @brief	Dual Core USB host task
 *
 * This function creates the task that invokes the host tasks
 * and the USB_USBTask() provided by the USB library. *This
 * function is called by @link EXAMPLE_DUALCORE_CMN_MAIN main()@endlink
 * only when EXAMPLE_USB_HOST is defined*.
 *
 * @return None
 */

int M0Image_Boot(uint32_t m0_image_addr);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif






