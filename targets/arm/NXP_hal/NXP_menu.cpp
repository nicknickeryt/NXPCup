/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * NXP HALina implementation of menu
 *
*/

#include "NXP_Kitty.hpp"

bool NXP_Menu::button0Flag = false;
bool NXP_Menu::button1Flag = false;
bool NXP_Menu::button2Flag = false;
bool NXP_Menu::button3Flag = false;


void NXP_Menu::init() {
    buttons.init();
    switches.init();
    display.print("----");
}

void NXP_Menu::proc(){
    while(1){
        display.update();
        if(button0Flag){
            display.print("--1-");
            button0Flag = false;
        }
        if(button1Flag){
            display.print("--2-");
            button1Flag = false;
        }
        if(button2Flag){
            display.print("--3-");
            button2Flag = false;
        }
        if(button3Flag){
            display.print("--4-");
            button3Flag = false;
            break;
        }

    }
}