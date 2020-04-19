 #include "NXP_CGU.hpp"

static void delayus(uint32_t us) {
    for (volatile uint32_t i=0; i<us; i++) {
        for (volatile uint32_t j=0; j<38; j++);
    }
}

void clockInit() {
    __disable_irq();

    // setup CLKOUT
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_PLL1, NXP_CGU::ENTITY::CLKSRC_IDIVB);
    NXP_CGU::enableEntity(NXP_CGU::ENTITY::CLKSRC_IDIVB, true);
    NXP_CGU::setDIV(NXP_CGU::ENTITY::CLKSRC_IDIVB, 12);  // 12 -> 6 pclks per cpu clk, 10 -> 5 pclks

    // set input for CLKOUT to IDIVB
    LPC_CGU->BASE_CLK[CLK_BASE_OUT] &= ~ CGU_BASE_OUT_CLK_CLK_SEL_MASK;
    LPC_CGU->BASE_CLK[CLK_BASE_OUT] |=  CGU_BASE_OUT_CLK_CLK_SEL(0xD);

    __enable_irq();
}

void timerInit() {
    // set timer so we count clock cycles
    LPC_TIMER1->IR = 0;
    LPC_TIMER1->TCR = 1;
    LPC_TIMER1->PR = 0;

    // microsecond timer
    LPC_TIMER2->IR = 0;
    LPC_TIMER2->TCR = 1;
    LPC_TIMER2->PR = 204-1; // 204MHz core clock
}