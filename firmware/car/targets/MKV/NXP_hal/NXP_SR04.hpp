#pragma once

#include "NXP_gpio.hpp"
#include "NXP_PIT.hpp"



class NXP_SR04 {
public:
    NXP_SR04(NXP_GPIO trigger,
             NXP_GPIO echo,
             NXP_PIT::CHANNEL pitChannel);

    void init();
    void pitHandler();
    void echoHandler();

    uint32_t getDistanceMm();
    bool isReady();    
    // ISR entry points
    static void pitCallback(uint32_t*);
    static void echoCallback();

private:
    // hardware
    NXP_GPIO triggerPin;
    NXP_GPIO echoPin;
    NXP_PIT  pitTimer;

    // timing
    volatile uint64_t ticks10us = 0;

    volatile uint64_t echoStart = 0;
    volatile uint64_t echoEnd   = 0;

    volatile uint32_t distanceMm = 0;
    volatile bool dataReady = false;

    // trigger state machine
    volatile uint32_t delayTicks = 0;
    volatile uint32_t pulseTicks = 0;
    volatile bool pulseActive = false;

    static NXP_SR04* instance; // do ISR
};


