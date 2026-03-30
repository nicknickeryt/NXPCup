#include "NXP_SR04.hpp"

NXP_SR04* NXP_SR04::instance = nullptr;

NXP_SR04::NXP_SR04(NXP_GPIO trigger, NXP_GPIO echo, NXP_PIT::CHANNEL pitChannel) :
    triggerPin(trigger),
    echoPin(echo),
    pitTimer(pitChannel,
             100000, // 10us tick
             NXP_SR04::pitCallback,
             nullptr) {}

void NXP_SR04::pitCallback(uint32_t*) {
    if (instance) instance->pitHandler();
}

void NXP_SR04::echoCallback() {
    if (instance) instance->echoHandler();
}

void NXP_SR04::init() {
    instance = this;

    triggerPin.init();
    triggerPin.reset();

    echoPin.init();

    pitTimer.init();
}

// note that with 2ms between pulses we can measure up to 34cm!
void NXP_SR04::pitHandler() {
    ticks10us  = ticks10us + 1;
    delayTicks = delayTicks + 1;

    if (!pulseActive) {
        if (delayTicks >= 5000) { // 30 ms
            triggerPin.set();
            pulseActive = true;

            pulseTicks = 0;
            delayTicks = 0;
        }
    } else {
        pulseTicks = pulseTicks + 1;

        if (pulseTicks >= 1) { // 10 µs
            triggerPin.reset();
            pulseActive = false;
        }
    }

    if (echoWaiting) {
        uint64_t now = ticks10us;

        uint64_t elapsed = (now >= echoStartTick) ? (now - echoStartTick) : 0;

        // echoTimeotMs = 25 ms → 2500 ticków (bo 10 µs)
        uint64_t timeoutTicks = echoTimeotMs * 100;

        if (elapsed > timeoutTicks) {
            // timeout = brak echa
            echoWaiting = false;
            distanceMm  = 0xFFFF; // albo np. 5000 mm
            dataReady   = true;
        }
    }
}

void NXP_SR04::echoHandler() {
    bool     state = echoPin.get();
    uint64_t now   = ticks10us;

    if (state) {
        echoStart     = now;
        echoStartTick = now;
        echoWaiting   = true;
    } else {
        echoEnd     = now;
        echoWaiting = false;

        uint64_t delta = (echoEnd >= echoStart) ? (echoEnd - echoStart) : 0;

        if (delta == 0 || delta < 5) return;

        uint32_t us = delta * 10;
        distanceMm  = (us * 10 + 29) / 58;

        dataReady = true;
    }
}

uint16_t NXP_SR04::getDistanceMm() {
    dataReady = false;
    return distanceMm;
}

bool NXP_SR04::isReady() { return dataReady; }
