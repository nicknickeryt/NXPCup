/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of 7-segment display driver
 *
 */

#pragma once
#include "HALina.hpp"
#include "HALina_config.hpp"

    class NXP_Display{

    private:
        uint8_t displayBuffer[4] = {0,0,0,0};

    public:
        NXP_Display()  {
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
        }

        void init();

        void print(const char*);

        void print(uint32_t);

        void update();

        static void A1_On()  { DISPLAY_GPIO->PCOR = DISPLAY_A1; }
        static void A2_On()  { DISPLAY_GPIO->PCOR = DISPLAY_A2; }
        static void A3_On()  { DISPLAY_GPIO->PCOR = DISPLAY_A3; }
        static void A4_On()  { DISPLAY_GPIO->PCOR = DISPLAY_A4; }

        static void A1_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_A1; }
        static void A2_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_A2; }
        static void A3_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_A3; }
        static void A4_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_A4; }

        static void A1_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_A1; }
        static void A2_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_A2; }
        static void A3_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_A3; }
        static void A4_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_A4; }

        static void A_On()  { DISPLAY_GPIO->PCOR = DISPLAY_A; }
        static void B_On()  { DISPLAY_GPIO->PCOR = DISPLAY_B; }
        static void C_On()  { DISPLAY_GPIO->PCOR = DISPLAY_C; }
        static void D_On()  { DISPLAY_GPIO->PCOR = DISPLAY_D; }
        static void E_On()  { DISPLAY_GPIO->PCOR = DISPLAY_E; }
        static void F_On()  { DISPLAY_GPIO->PCOR = DISPLAY_F; }
        static void G_On()  { DISPLAY_GPIO->PCOR = DISPLAY_G; }
        static void DP_On()  { DISPLAY_GPIO->PCOR = DISPLAY_DP; }

        static void A_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_A; }
        static void B_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_B; }
        static void C_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_C; }
        static void D_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_D; }
        static void E_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_E; }
        static void F_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_F; }
        static void G_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_G; }
        static void DP_Off()  { DISPLAY_GPIO->PSOR = DISPLAY_DP; }

        static void A_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_A; }
        static void B_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_B; }
        static void C_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_C; }
        static void D_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_D; }
        static void E_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_E; }
        static void F_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_F; }
        static void G_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_G; }
        static void DP_Toggle()  { DISPLAY_GPIO->PTOR = DISPLAY_DP; }

        static void signsOff()  { DISPLAY_GPIO->PSOR = DISPLAY_A1 | DISPLAY_A2 | DISPLAY_A3 | DISPLAY_A4; }
        static void segmentsOff()  { DISPLAY_GPIO->PSOR = DISPLAY_A | DISPLAY_B | DISPLAY_C | DISPLAY_D | DISPLAY_E | DISPLAY_F | DISPLAY_G | DISPLAY_DP; }
    };
