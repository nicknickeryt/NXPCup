/* General includes */
#include <stdio.h>
#include "dualcore_common.h"
#include "ipc_msg.h"

#define IPCEX_ID_BLINKY 4
static const uint32_t xDelay = 500;

static void LED_blinkProc(uint32_t val) {
	Board_LED_Set((val >> 16) & 0xFFFF, val & 0xFFFF);
}

static int blink_delay(void) {
	static int32_t final, init;
	if (!init) {
		int32_t curr = (int32_t) Chip_RIT_GetCounter(LPC_RITIMER);
		final = curr + (SystemCoreClock / 1000) * xDelay;
		init = 1 + (final < 0 && curr > 0);
	}

	if ((init == 2 && Chip_RIT_GetCounter(LPC_RITIMER) >= (uint32_t) final) ||
		(init == 1 && (int32_t) Chip_RIT_GetCounter(LPC_RITIMER) >= final)) {
			init = 0;
	}
	return init != 0;
}

int main(void) {
	SystemCoreClockUpdate();

	IPCEX_Init(); //Initialize the IPC Queue

	ipcex_register_callback(IPCEX_ID_BLINKY, LED_blinkProc);

	DEBUGSTR("Starting M0 Tasks...\r\n");

	while(1) {
		ipcex_tasks();

		static int blink = 0;

		if (!blink_delay()) {
			if (ipcex_msgPush(IPCEX_ID_BLINKY, (0 << 16) | blink) == QUEUE_INSERT) {
				blink = 1 - blink;
			}
		}
	}
	return 0;
}
