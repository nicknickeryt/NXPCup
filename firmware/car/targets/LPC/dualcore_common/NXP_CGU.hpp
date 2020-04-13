#include <cstdint>
#include "NXP_SCU.hpp"
#include "chip.h"

#define CGU_ADDRESS32(x,y) (*(uint32_t*)((uint32_t)x+y))
#define CGU_CGU_ADDR	((uint32_t)LPC_CGU)

/** Check bit at specific position is set or not */
#define ISBITSET(x,bit) 			(x&(1<<bit))
/** Check bit at specific position is clear or not */
#define ISBITCLR(x,bit) 			((x&(1<<bit))^(1<<bit))

class NXP_CGU {
public:
    enum class ENTITY : uint8_t {
        // Clock Source
        CLKSRC_32KHZ_OSC = 0,					/**< 32KHz oscillator clock source 	*/
        CLKSRC_IRC,								/**< IRC 12 Mhz clock source 		*/
        CLKSRC_ENET_RX_CLK,						/**< Ethernet receive clock source 	*/
        CLKSRC_ENET_TX_CLK,						/**< Ethernet transmit clock source */
        CLKSRC_GP_CLKIN,						/**< General purpose clock source 	*/
        CLKSRC_TCK,								/**< TCK clock source 				*/
        CLKSRC_XTAL_OSC,						/**< Crystal oscillator clock source*/
        CLKSRC_PLL0,							/**< PLL0 (USB0) clock source		*/
        CLKSRC_PLL0_AUDIO,
        CLKSRC_PLL1,							/**< PLL1 clock source				*/
        CLKSRC_IDIVA = CLKSRC_PLL1 + 3,		/**< IDIVA clock source				*/
        CLKSRC_IDIVB,							/**< IDIVB clock source				*/
        CLKSRC_IDIVC,							/**< IDIVC clock source				*/
        CLKSRC_IDIVD,							/**< IDIVD clock source				*/
        CLKSRC_IDIVE,							/**< IDIVE clock source				*/

        // Base
        BASE_SAFE,								/**< Base save clock (always on) for WDT */
        BASE_USB0,								/**< USB0 base clock 				*/
        BASE_USB1 = BASE_USB0 + 2,			/**< USB1 base clock 				*/
        BASE_M0,								/**< ARM Cortex-M0 Core base clock 	*/
        BASE_SPIFI,								/**< SPIFI base clock				*/
        U_BASE_SPI,
        BASE_PHY_RX = BASE_SPIFI + 2,		/**< Ethernet PHY Rx base clock		*/
        BASE_PHY_TX,							/**< Ethernet PHY Tx base clock		*/
        BASE_APB1,								/**< APB peripheral block #1 base clock	*/
        BASE_APB3,								/**< APB peripheral block #3 base clock	*/
        BASE_LCD,								/**< LCD base clock					*/
        BASE_ENET_CSR,
        BASE_SDIO,								/**< SDIO base clock				*/
        BASE_SSP0,								/**< SSP0 base clock				*/
        BASE_SSP1,								/**< SSP1 base clock				*/
        BASE_UART0,								/**< UART0 base clock				*/
        BASE_UART1,								/**< UART1 base clock				*/
        BASE_UART2,								/**< UART2 base clock				*/
        BASE_UART3,								/**< UART3 base clock				*/
        BASE_CLKOUT,							/**< CLKOUT base clock				*/
        BASE_APLL = BASE_CLKOUT + 5,
        BASE_OUT0,
        BASE_OUT1,
        ENTITY_NUM								/**< Number or clock source entity	*/
    };
    /** CGU number of clock source */
    constexpr static uint8_t  CLKSRC_NUM = static_cast<uint8_t>(ENTITY::CLKSRC_IDIVE) + 1;
    inline static uint32_t CGU_ClockSourceFrequency[CLKSRC_NUM] = {0, 12000000, 0, 0, 0, 0, 0, 480000000, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    constexpr static const int16_t CGU_Entity_ControlReg_Offset[static_cast<int>(ENTITY::ENTITY_NUM)] = {
            -1,		//CGU_CLKSRC_32KHZ_OSC,
            -1,		//CGU_CLKSRC_IRC,
            -1,		//CGU_CLKSRC_ENET_RX_CLK,
            -1,		//CGU_CLKSRC_ENET_TX_CLK,
            -1,		//CGU_CLKSRC_GP_CLKIN,
            -1,		//CGU_CLKSRC_TCK,
            0x18,	//CGU_CLKSRC_XTAL_OSC,
            0x20,	//CGU_CLKSRC_PLL0,
            0x30,	//CGU_CLKSRC_PLL0_AUDIO **REV A**
            0x44,	//CGU_CLKSRC_PLL1,
            -1,		//CGU_CLKSRC_RESERVE,
            -1,		//CGU_CLKSRC_RESERVE,
            0x48,	//CGU_CLKSRC_IDIVA,,
            0x4C,	//CGU_CLKSRC_IDIVB,
            0x50,	//CGU_CLKSRC_IDIVC,
            0x54,	//CGU_CLKSRC_IDIVD,
            0x58,	//CGU_CLKSRC_IDIVE,

            0x5C,	//CGU_BASE_SAFE,
            0x60,	//CGU_BASE_USB0,
            -1,		//CGU_BASE_RESERVE,
            0x68,	//CGU_BASE_USB1,
            0x6C,	//CGU_BASE_M3,
            0x70,	//CGU_BASE_SPIFI,
            -1,		//CGU_BASE_RESERVE,
            0x78,	//CGU_BASE_PHY_RX,
            0x7C,	//CGU_BASE_PHY_TX,
            0x80,	//CGU_BASE_APB1,
            0x84,	//CGU_BASE_APB3,
            0x88,	//CGU_BASE_LCD,
            0X8C,	//CGU_BASE_ENET_CSR, **REV A**
            0x90,	//CGU_BASE_SDIO,
            0x94,	//CGU_BASE_SSP0,
            0x98,	//CGU_BASE_SSP1,
            0x9C,	//CGU_BASE_UART0,
            0xA0,	//CGU_BASE_UART1,
            0xA4,	//CGU_BASE_UART2,
            0xA8,	//CGU_BASE_UART3,
            0xAC,	//CGU_BASE_CLKOUT
            -1,
            -1,
            -1,
            -1,
            0xC0,	//CGU_BASE_APLL
            0xC4,	//CGU_BASE_OUT0
            0xC8	//CGU_BASE_OUT1
    };

    constexpr static const uint8_t CGU_ConnectAlloc_Tbl[CLKSRC_NUM][static_cast<uint8_t>(ENTITY::ENTITY_NUM)] = {
    //       3 I E E G T X P P P x x D D D D D S U x U M S x P P A A L E S S S U U U U C x x x x A O O
    //       2 R R T P C T L L L     I I I I I A S   S 3 P   H H P P C N D S S R R R R O         P U U
    //         C X X I K A 0 A 1     A B C D E F B   B   F   RxTx1 3 D T I 0 1 0 1 2 3           L T T
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_32KHZ_OSC = 0,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_IRC,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_ENET_RX_CLK,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_ENET_TX_CLK,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_GP_CLKIN,*/
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},/*CGU_CLKSRC_TCK,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_XTAL_OSC,*/
            {0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1},/*CGU_CLKSRC_PLL0,*/
            {0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_PLL0_AUDIO,*/
            {0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_PLL1,*/
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_IDIVA = CGU_CLKSRC_PLL1 + 3,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_IDIVB,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_IDIVC,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1},/*CGU_CLKSRC_IDIVD,*/
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1}/*CGU_CLKSRC_IDIVE,*/
    };

    enum class STATUS {
        SUCCESS = 0,
        ERROR_CONNECT_TOGETHER,
        INVALID_ENTITY,
        INVALID_CLOCK_SOURCE,
        INVALID_PARAM,
        FREQ_OUTOF_RANGE
    };

    static STATUS SetXTALOSC(uint32_t ClockFrequency){
        if (ClockFrequency < 15000000){
            LPC_CGU->XTAL_OSC_CTRL &= ~ CGU_XTAL_OSC_CTRL_HF_MASK;
        } else if (ClockFrequency < 25000000) {
            LPC_CGU->XTAL_OSC_CTRL |= CGU_XTAL_OSC_CTRL_HF_MASK;
        } else {
            return STATUS::FREQ_OUTOF_RANGE;
        }
        CGU_ClockSourceFrequency[static_cast<uint8_t>(ENTITY::CLKSRC_XTAL_OSC)] = ClockFrequency;
        return STATUS::SUCCESS;
    }

    static STATUS enableEntity(ENTITY entity, bool enable) {
        if (entity == ENTITY::CLKSRC_32KHZ_OSC) {
            if (enable) {
                LPC_CREG->CREG0 &= ~( CREG_CREG0_PD32KHZ_MASK | CREG_CREG0_RESET32KHZ_MASK);
                LPC_CREG->CREG0 |= CREG_CREG0_EN32KHZ_MASK | CREG_CREG0_EN1KHZ_MASK;
            } else {
                LPC_CREG->CREG0 &= ~(CREG_CREG0_EN32KHZ_MASK | CREG_CREG0_EN1KHZ_MASK);
                LPC_CREG->CREG0 |= CREG_CREG0_PD32KHZ_MASK;
            }
            for(volatile uint32_t  i = 0; i<1000000; i++);
        } else if (entity == ENTITY::CLKSRC_ENET_RX_CLK) {
            NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_3, SCU_MODE_INACT, 0xC, 0);
        } else if (entity == ENTITY::CLKSRC_ENET_TX_CLK) {
            NXP_SCU::enable(NXP_SCU::Type::PIN, NXP_SCU::Function::_0, SCU_MODE_INACT, 0x1, 19);
        } else if (entity == ENTITY::CLKSRC_GP_CLKIN){

        } else if (entity == ENTITY::CLKSRC_TCK){

        } else if (entity == ENTITY::CLKSRC_XTAL_OSC){
            if(!enable)
                LPC_CGU->XTAL_OSC_CTRL |= CGU_XTAL_OSC_CTRL_ENABLE_MASK;
            else
                LPC_CGU->XTAL_OSC_CTRL &= ~CGU_XTAL_OSC_CTRL_ENABLE_MASK;
            /*Delay for stable clock*/
            for(volatile uint32_t i = 0; i<1000000; i++);
        } else {
            int16_t RegOffset = CGU_Entity_ControlReg_Offset[static_cast<uint8_t >(entity)];
            if (RegOffset == -1) return STATUS::INVALID_ENTITY;
            if (!enable){
                CGU_ADDRESS32(CGU_CGU_ADDR,RegOffset) |= CGU_CTRL_ENABLE_MASK;
            } else {
                CGU_ADDRESS32(CGU_CGU_ADDR,RegOffset) &= ~CGU_CTRL_ENABLE_MASK;
                /*if PLL is selected check if it is locked */
                if(entity == ENTITY::CLKSRC_PLL0){
                    while((LPC_CGU->PLL[CGU_USB_PLL].PLL_STAT & 1) == 0x0);
                }
                if(entity == ENTITY::CLKSRC_PLL1) {
                    while((LPC_CGU->PLL1_STAT & 1) == 0x0);
                    /*post check lock status */
                    if(!(LPC_CGU->PLL1_STAT & 1)) {
                        while(1);
                    }
                }
            }
        }

        return STATUS::SUCCESS;
    }

    /* Connect one entity into clock source */
    static STATUS entityConnect(ENTITY ClockSource, ENTITY ClockEntity){
        int32_t RegOffset;
        uint32_t tempReg;

        if (ClockSource > ENTITY::CLKSRC_IDIVE) {
            return STATUS::INVALID_CLOCK_SOURCE;
        }
        if (ClockEntity >= ENTITY::CLKSRC_PLL0 && ClockEntity <= ENTITY::BASE_CLKOUT){
            if (CGU_ConnectAlloc_Tbl[static_cast<uint8_t>(ClockSource)][static_cast<uint8_t>(ClockEntity)]) {
                RegOffset = CGU_Entity_ControlReg_Offset[static_cast<uint8_t>(ClockSource)];
                if (RegOffset != -1) {
                    if (ClockEntity <= ENTITY::CLKSRC_IDIVE && ClockEntity >= ENTITY::CLKSRC_PLL0) {
                        //RegOffset = (CGU_ADDRESS32(LPC_CGU,RegOffset)>>24)&0xF;
                        if(static_cast<ENTITY>((CGU_ADDRESS32(LPC_CGU, RegOffset) >> 24u) & 0xFu) == ClockEntity) {
                            return STATUS::ERROR_CONNECT_TOGETHER;
                        }

                    }
                }
                RegOffset = CGU_Entity_ControlReg_Offset[static_cast<uint8_t>(ClockEntity)];
                if (RegOffset == -1) {
                    return STATUS::INVALID_ENTITY;
                }
                tempReg = CGU_ADDRESS32(LPC_CGU, RegOffset);
                tempReg &= ~ CGU_CTRL_CLK_SEL_MASK;
                tempReg |= static_cast<uint32_t>(ClockSource) << 24u | CGU_CTRL_AUTOBLOCK_MASK;
                CGU_ADDRESS32(LPC_CGU,RegOffset) = tempReg;
                return STATUS::SUCCESS;
            } else {
                return STATUS::INVALID_CLOCK_SOURCE;
            }

        } else {
            return STATUS::INVALID_ENTITY;
        }
    }

    static STATUS setPLL1(uint32_t mult){
        // TODO to chyba ma być inaczej
        uint32_t msel=0, nsel=0, psel=0, pval=1;
        uint32_t freq;
        uint32_t ClkSrc = (LPC_CGU->PLL1_CTRL & CGU_CTRL_CLK_SEL_MASK) >> 24u;
        freq = CGU_ClockSourceFrequency[ClkSrc];
        freq *= mult;
        msel = mult-1;

        LPC_CGU->PLL1_CTRL &= ~( CGU_PLL1_CTRL_FBSEL_MASK | CGU_PLL1_CTRL_BYPASS_MASK | CGU_PLL1_CTRL_DIRECT_MASK |
                                 CGU_PLL1_CTRL_PSEL_MASK  | CGU_PLL1_CTRL_MSEL_MASK   | CGU_PLL1_CTRL_NSEL_MASK );
        if (freq<156000000) {
            //psel is encoded such that 0=1, 1=2, 2=4, 3=8
            while(2*(pval)*freq < 156000000) {
                psel++;
                pval*=2;
            }
//		if(2*(pval)*freq > 320000000) {
//			//THIS IS OUT OF RANGE!!!
//			//HOW DO WE ASSERT IN SAMPLE CODE?
//			//__breakpoint(0);
//			return CGU_ERROR_INVALID_PARAM;
//		}
            LPC_CGU->PLL1_CTRL |= CGU_PLL1_CTRL_MSEL(msel) | CGU_PLL1_CTRL_NSEL(nsel) | CGU_PLL1_CTRL_PSEL(psel) | CGU_PLL1_CTRL_FBSEL_MASK;
        } else if(freq < 320000000) {
            LPC_CGU->PLL1_CTRL |= CGU_PLL1_CTRL_MSEL(msel) | CGU_PLL1_CTRL_NSEL(nsel) | CGU_PLL1_CTRL_PSEL(psel) | CGU_PLL1_CTRL_DIRECT_MASK | CGU_PLL1_CTRL_FBSEL_MASK;
        } else {
            return STATUS::INVALID_PARAM;
        }
        return STATUS::SUCCESS;
    }

    /* divisor number must >=1*/
    static STATUS setDIV(ENTITY SelectDivider, uint32_t divisor) {
        int32_t RegOffset;
        uint32_t tempReg;
        if(SelectDivider >= ENTITY::CLKSRC_IDIVA && SelectDivider <= ENTITY::CLKSRC_IDIVE){
            RegOffset = CGU_Entity_ControlReg_Offset[static_cast<uint8_t>(SelectDivider)];
            if(RegOffset == -1) return STATUS::INVALID_ENTITY;
            tempReg = CGU_ADDRESS32(LPC_CGU, RegOffset);
            tempReg &= ~(0xFFu << 2u);
            tempReg |= ((divisor-1) & 0xFFu) << 2u;
            CGU_ADDRESS32(LPC_CGU,RegOffset) = tempReg;
            return STATUS::SUCCESS;
        }
        return STATUS::INVALID_ENTITY;
    }

    static void updateClock(){
        uint32_t ClkSrc;
        uint32_t div;
        uint32_t divisor;
        int32_t RegOffset;
        /* 32OSC */
        if(ISBITSET(LPC_CREG->CREG0,1) && ISBITCLR(LPC_CREG->CREG0,3))
            CGU_ClockSourceFrequency[static_cast<uint8_t >(ENTITY::CLKSRC_32KHZ_OSC)] = 32768;
        else
            CGU_ClockSourceFrequency[static_cast<uint8_t >(ENTITY::CLKSRC_32KHZ_OSC)] = 0;
        /*PLL0*/
        /* PLL1 *//* Enabled */
        if(ISBITCLR(LPC_CGU->PLL1_CTRL,1)  && (LPC_CGU->PLL1_STAT&1)) { /* Locked? */
            ClkSrc = (LPC_CGU->PLL1_CTRL & CGU_CTRL_CLK_SEL_MASK) >> 24u;
            CGU_ClockSourceFrequency[static_cast<uint8_t >(ENTITY::CLKSRC_PLL1)] =
                    CGU_ClockSourceFrequency[ClkSrc] * (((LPC_CGU->PLL1_CTRL>>16u)&0xFFu)+1);
        } else {
            CGU_ClockSourceFrequency[static_cast<uint8_t >(ENTITY::CLKSRC_PLL1)] = 0;
        }

        /* DIV */
        for(div = static_cast<uint8_t >(ENTITY::CLKSRC_IDIVA); div <= static_cast<uint8_t >(ENTITY::CLKSRC_IDIVE); div++){
            RegOffset = CGU_Entity_ControlReg_Offset[div];
            if (ISBITCLR(CGU_ADDRESS32(LPC_CGU,RegOffset),1)) {
                ClkSrc = (CGU_ADDRESS32(LPC_CGU,RegOffset) & CGU_CTRL_CLK_SEL_MASK) >> 24u;
                divisor = (CGU_ADDRESS32(LPC_CGU,RegOffset)>>2u) & 0xFFu;
                divisor ++;
                CGU_ClockSourceFrequency[div] = CGU_ClockSourceFrequency[ClkSrc] / divisor;
            } else {
                CGU_ClockSourceFrequency[div] = 0;
            }
        }
    }
};