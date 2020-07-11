#include <cstdio>
#include "chip.h"
#include "IPC.hpp"
#include "NXP_GPIO.hpp"
#include "NXP_CGU.hpp"
#include "Camera.hpp"
#include "init.hpp"
#include "usbuser.h"
#include "usblink.h"
#include "NXP_UART_M4.hpp"

static const uint32_t xDelay = 99;

void MSleep(int32_t msecs) {


	auto curr = (int32_t) Chip_RIT_GetCounter(LPC_RITIMER);
	int32_t final = curr + ((SystemCoreClock / 1000) * msecs);

	/* If the value is zero let us not worry about it */
	if (!msecs || (msecs < 0)) {
		return;
	}

	if ((final < 0) && (curr > 0)) {
		while (Chip_RIT_GetCounter(LPC_RITIMER) < (uint32_t) final) {}
	} else {
		while ((int32_t) Chip_RIT_GetCounter(LPC_RITIMER) < final) {}
	}
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

volatile bool irq = false;
Event event_global ;
void callbackIPC(Event event) {
    event_global = event;
    irq = true;
}

extern "C" {
void Board_SystemInit(void);
}

Camera camera;
USBLink usblink;
const char myData[] = "to sa moje nowe dane";
uint8_t length = sizeof(myData);

int main() {
    auto *pSCB_VTOR = (unsigned int *) 0xE000ED08;
    fpuInit();
    Board_SystemInit();

    IPC<CPU::M4> ipc {SHARED_MEM_M4, SHARED_MEM_M0, callbackIPC};
    ipc.init();
	SystemCoreClockUpdate();

	/* Make sure the M0 core is being held in reset via the RGU */
	Chip_RGU_TriggerReset(RGU_M0APP_RST);

	Chip_Clock_Enable(CLK_M4_M0APP);

	/* Keep in mind the M0 image must be aligned on a 4K boundary */
	Chip_CREG_SetM0AppMemMap(0x10080000); // m0_image_addr

	Chip_RGU_ClearReset(RGU_M0APP_RST);

	uint8_t index = 0;



    NXP_GPIO LED_green = {1,12};
    NXP_GPIO LED_blue = {1,11};

    LED_blue.init();
    LED_green.init();

//    LED_blue.reset();
//    LED_blue.set();

//    camera.init();
    timerInit();

    NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_2,
                    NXP_SCU::Mode_PULLDOWN,
                    0x6, 4);

    NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_2,
                    NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS,
                    0x6, 5);

//    NXP_UART uart(LPC_UART1, 115200);
    NXP_UART uart(LPC_USART0, 115200);
    uart.init();

    uart.write("kupa", 4);
    uart.write_u(1000);

    volatile uint32_t ddd = LPC_TIMER1->TC;

    NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_1,
                    NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN,
                    0x8, 1);
    NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_1,
                    NXP_SCU::Mode_INACT | NXP_SCU::Mode_INBUFF_EN | NXP_SCU::Mode_ZIF_DIS | NXP_SCU::Mode_HIGHSPEEDSLEW_EN,
                    0x8, 2);
    USB_UserInit();
    LPC_TIMER1->TC = 0;
//    LED_blue.reset();
//    LED_blue.reset();
    LED_green.set();
	while(true) {
        ddd = LPC_TIMER1->TC;
	    if (ddd < 10000000) {
            continue;
	    }


		if (!blink_delay()) {
            ipc.push(index++);
		}

        if (irq) {
            irq = false;
//            LED_blue.toggle();
            LED_green.toggle();
            usblink.send(reinterpret_cast<const uint8_t *>(myData), length, 100);
        }
	}
	return 0;
}
