 #include "NXP_CGU.hpp"

static void delayus(uint32_t us) {
    for (volatile uint32_t i=0; i<us; i++) {
        for (volatile uint32_t j=0; j<38; j++);
    }
}

void clockInit() {
    __disable_irq();
    // Set the XTAL oscillator frequency to 12MHz
    NXP_CGU::SetXTALOSC(12000000UL);
    NXP_CGU::enableEntity(NXP_CGU::ENTITY::CLKSRC_XTAL_OSC, true);
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_XTAL_OSC, NXP_CGU::ENTITY::BASE_M0);

    // Set PL160M 12*1 = 12 MHz
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_XTAL_OSC, NXP_CGU::ENTITY::CLKSRC_PLL1);

    NXP_CGU::setPLL1(1);
    NXP_CGU::enableEntity(NXP_CGU::ENTITY::CLKSRC_PLL1, true);

    // setup CLKOUT
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_PLL1, NXP_CGU::ENTITY::CLKSRC_IDIVB);
    NXP_CGU::enableEntity(NXP_CGU::ENTITY::CLKSRC_IDIVB, true);
    NXP_CGU::setDIV(NXP_CGU::ENTITY::CLKSRC_IDIVB, 12);  // 12 -> 6 pclks per cpu clk, 10 -> 5 pclks

    // set input for CLKOUT to IDIVB
    LPC_CGU->BASE_CLK[CLK_BASE_OUT] &= ~ CGU_BASE_OUT_CLK_CLK_SEL_MASK;
    LPC_CGU->BASE_CLK[CLK_BASE_OUT] &= ~ CGU_BASE_OUT_CLK_CLK_SEL(0xD);

    // Run SPIFI from PL160M
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_PLL1, NXP_CGU::ENTITY::CLKSRC_IDIVA);
    NXP_CGU::enableEntity(NXP_CGU::ENTITY::CLKSRC_IDIVA, true);
    NXP_CGU::setDIV(NXP_CGU::ENTITY::CLKSRC_IDIVA, 2);
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_IDIVA, NXP_CGU::ENTITY::BASE_SPIFI);
    NXP_CGU::updateClock();

    LPC_CCU1->CLKCCU[CLK_MX_EMC_DIV].CFG = (1u << 0u) |  (1u << 5u);	// Turn on clock / 2
    LPC_CREG->CREG6 |= CREG_CREG6_EMC_CLK_SEL_MASK;	    // EMC divided by 2
    LPC_CCU1->CLKCCU[CLK_MX_EMC].CFG |= CCU_CLK_XXX_CFG_RUN_SHIFT;		// Turn on clock


    NXP_CGU::setPLL1(9);
    /* Run base M3 clock from PL160M, no division */
    NXP_CGU::entityConnect(NXP_CGU::ENTITY::CLKSRC_PLL1, NXP_CGU::ENTITY::BASE_M0);

    delayus(10000);

    NXP_CGU::setPLL1(17);
    delayus(10000);

    NXP_CGU::updateClock();

    __enable_irq();
}