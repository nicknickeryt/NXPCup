#include "board.h"
#include "IPC.h"

static const uint32_t xDelay = 500;

static void LED_blinkProc(uint32_t val) {
    volatile uint32_t d = val;
    asm("nop");
	Board_LED_Set((val >> 16) & 0xFFFF, val & 0xFFFF);
}

static int blink_delay() {
	static int32_t final, init;
	if (!init) {
		auto curr = (int32_t) Chip_RIT_GetCounter(LPC_RITIMER);
		final = curr + (SystemCoreClock / 1000) * xDelay;
		init = 1 + (final < 0 && curr > 0);
	}

	if ((init == 2 && Chip_RIT_GetCounter(LPC_RITIMER) >= (uint32_t) final) ||
		(init == 1 && (int32_t) Chip_RIT_GetCounter(LPC_RITIMER) >= final)) {
			init = 0;
	}
	return init != 0;
}

int main() {
    IPC<CPU::M0> ipc {SHARED_MEM_M0, SHARED_MEM_M4};
    ipc.setNVIC();
	SystemCoreClockUpdate();

	DEBUGSTR("Starting M0 Tasksssss...\r\n");

	while(true) {
	    static int i = 0;
	    i++;

		static int blink = 0;

		if (!blink_delay()) {
		    ipc.sendSignal();
//            Board_LED_Toggle(0);
		}
	}
	return 0;
}
