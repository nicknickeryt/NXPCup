#define LOG_CHANNEL KITTY

#include "MKV_Kitty.hpp"

#include "clock_config.h"
#include "pin_mux.h"

#include "logger.h"

bool commandTerminalTrigger = false;
bool frameTrigger           = false;

void pit_generalHandler(uint32_t*) {
    commandTerminalTrigger = true;
    frameTrigger           = true;
}

static void logWrite(char c, [[maybe_unused]] void* const context) {
    Kitty::kitty().uartDebug.write(c);
}

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    // TODO
    // FTM_Init();

    uartDebug.init();
    log_setWriteFunction(logWrite);
    uartCommunication.init();
    uartCommunication.initDMA();
    uartToKLZ.init();

    ledLine.init();

    display.init();

    servo.init();

    camera.init();

    pitCamera.init();

    pitSendCameraData.init();

    motors.init();

    uartDebug.init();



    uartCommunication.write("Kitty initializing...", 19);

    motors.run();
    camera.start();
    display.enable();
    ledLine.at(0).set();
    display.print("1234");
    servo.set(0.0);

    log_notice("KiTTy init finished");
}
