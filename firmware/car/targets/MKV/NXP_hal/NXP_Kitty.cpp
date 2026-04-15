/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#include "HALina_led_line.hpp"
#include "algorithms/motor.hpp"
#define LOG_CHANNEL KITTY

#include "logger.h"

#include "NXP_Kitty.hpp"
#include "clock_config.h"
#include "pin_mux.h"
#include "printf.h"
bool algorithmTrigger       = false;
bool commandTerminalTrigger = false;
bool frameTrigger           = false;

bool pixyTrigger = false;
void pit_generalHandler(uint32_t*) {
    algorithmTrigger       = true;
    commandTerminalTrigger = true;
    frameTrigger           = true;
    pixyTrigger            = true;
}

bool cameraDataReceived = false;

void logWrite(char c, [[maybe_unused]] void* const context) { Kitty::kitty().uartDebug.write(c); }

uint_fast64_t Kitty::milliseconds = 0;
extern "C" {
bool systickTrigger = false;
void SysTick_Handler(void) {
    Kitty::millisIncrease();
    systickTrigger = true;
}
}

float motorL = 0.0f;
float motorR = 0.0f;

void Kitty::uartCallback(uint8_t receivedByte) {
    if(!kitty().params.getUartEnabled()) return;
    switch (receivedByte) {
        case 's':
            // fctprintf(logWrite, NULL, "\nkittyStop\n", 0);
            // kitty().menu.setTriggeredOff(false);
            // kitty().motors.setValue(0, 0);
            // kitty().motors.setEnabled(false);
            // kitty().servo.set(0);
            // kitty().servo.disable();
            kitty().motors.setEnabled(false);
            break;
        case 'p':
            break;
        case 'o':
            kitty().params.setStartRPM(0.3);
            break;
        case 'r':
            kitty().menu.setTriggeredOff(true);
            kitty().motors.setEnabled(true);
            kitty().newAlgorithm.clearPatterns(millis());
            kitty().servo.init();
            kitty().menu.startRace(millis());
            kitty().differential.getLeftPID().reset();
            kitty().differential.getRightPID().reset();
            kitty().newAlgorithm.setAlgorithmStartTime(millis());
            break;

        case '+': // startVelocty++
            kitty().params.setStartRPM(kitty().params.getStartRPM() + 100);
            break;
        case '-': // startVelocity--
            kitty().params.setStartRPM(kitty().params.getStartRPM() - 100);
            break;

        case 'a': // brakeOne++
            kitty().params.setBrakeOne(kitty().params.getBrakeOne() + 0.5f);
            break;
        case 'b': // brakeOne--
            kitty().params.setBrakeOne(kitty().params.getBrakeOne() - 0.5f);
            break;

        case 'c': // brakeAll++
            kitty().params.setBrakeAll(kitty().params.getBrakeAll() + 0.5f);
            break;
        case 'd': // brakeAll--
            kitty().params.setBrakeAll(kitty().params.getBrakeAll() - 0.5f);
            break;
        
        case 'e': // servoDivider++
            kitty().params.setServoDivider(kitty().params.getServoDivider() + 0.5f);
            break;
        case 'f': // servoDivider--
            kitty().params.setServoDivider(kitty().params.getServoDivider() - 0.5f);
            break;

        case 'g': // pidKp++
            kitty().params.setPidKp(kitty().params.getPidKp() + 0.05f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());
            break;
        case 'h': // pidKp--
            kitty().params.setPidKp(kitty().params.getPidKp() - 0.05f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());            break;

        case 'i': // pidKi++
            kitty().params.setPidKi(kitty().params.getPidKi() + 0.0001f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());            break;
        case 'j': // pidKi--
            kitty().params.setPidKi(kitty().params.getPidKi() - 0.0001f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());
            break;

        case 'k': // filter++
            kitty().params.setAlgorithmFilterAlpha(kitty().params.getAlgorithmFilterAlpha() + 0.05f);
            break;
        case 'l': // filter--
            kitty().params.setAlgorithmFilterAlpha(kitty().params.getAlgorithmFilterAlpha() - 0.05f);
            break;

        case 'm': // brakeClamp++
            kitty().params.setBrakeClamp(kitty().params.getBrakeClamp() + 0.05f);
            break;
        case 'n': // brakeClamp--
            kitty().params.setBrakeClamp(kitty().params.getBrakeClamp() - 0.05f);
            break;

        case '5': // outsideRPM++
            kitty().params.setDiffClamp(kitty().params.getDiffClamp() + 0.05f);
            break;
        case '6': // outsideRPM--
            kitty().params.setDiffClamp(kitty().params.getDiffClamp() - 0.05f);
            break;

        case '7': // insideRPM++
            kitty().params.setRPMOffset(kitty().params.getRPMOffset() + 50);
            break;
        case '8': // insideRPM--
            kitty().params.setRPMOffset(kitty().params.getRPMOffset() - 50);
            break;

        case '9': // threshold++
            kitty().params.setPatternCorrelationThreshold(kitty().params.getPatternCorrelationThreshold() + 0.01f);
            break;
        case '0': // threshold--
            kitty().params.setPatternCorrelationThreshold(kitty().params.getPatternCorrelationThreshold() - 0.01f);
            break;


        case '!': // brighnessMultiplier++
            kitty().params.setCrossingsBrightnessMultiplier(kitty().params.getCrossingsBrightnessMultiplier() + 0.05f);
            break;
        case '@': // brighnessMultiplier--
            kitty().params.setCrossingsBrightnessMultiplier(kitty().params.getCrossingsBrightnessMultiplier() - 0.05f);
            break;
    
        case '#': // crossingCut++
            kitty().params.setCrossingsCut(kitty().params.getCrossingsCut() + 1);
            break;
        case '$': // crossingCut--
            kitty().params.setCrossingsCut(kitty().params.getCrossingsCut() - 1);
            break;

        case '%': // brightnessMeanAlpha++
            kitty().params.setBrightnessMeanAlpha(kitty().params.getBrightnessMeanAlpha() + 0.05f);
            break;
        case '^': // brightnessMeanAlpha--
            kitty().params.setBrightnessMeanAlpha(kitty().params.getBrightnessMeanAlpha() - 0.05f);
            break;

        case '&': // crossingAlpha++
            kitty().params.setCameraCrossingFilterAlpha(kitty().params.getCameraCrossingFilterAlpha() + 0.05f);
            break;
        case '*': // crossingAlpha--
            kitty().params.setCameraCrossingFilterAlpha(kitty().params.getCameraCrossingFilterAlpha() - 0.05f);
            break;

        case '(': // camFreq++
            kitty().params.setCameraPitFrequency(kitty().pitCamera, kitty().params.getCameraPitFrequency() + 1000);
            break;
        case ')': // camFreq--
            kitty().params.setCameraPitFrequency(kitty().pitCamera, kitty().params.getCameraPitFrequency() - 1000);
            break;

        case '{': // offset++
            kitty().params.setAlgorithmOffset(kitty().params.getAlgorithmOffset() + 0.1f);
            break;
        case '}': // offset--
            kitty().params.setAlgorithmOffset(kitty().params.getAlgorithmOffset() - 0.1f);
            break;

        case '[': // pattern timeout++
            kitty().params.setPatternDetectTimeoutMs(kitty().params.getPatternDetectTimeoutMs() + 250);
            break;
        case ']': // pattern timeout--
            kitty().params.setPatternDetectTimeoutMs(kitty().params.getPatternDetectTimeoutMs() - 250);
            break;

        
        case ':': // pattern stop enable
            kitty().params.setIsPatternStopEnabled(true);
            break;
        case ';': // pattern stop disable
            kitty().params.setIsPatternStopEnabled(false);
            break;

        case ',': // pidKd++
            kitty().params.setPidKd(kitty().params.getPidKd() + 0.005f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());
            break;
        case '.': // pidKd--
            kitty().params.setPidKd(kitty().params.getPidKd() - 0.005f, kitty().differential.getLeftPID(), kitty().differential.getRightPID());            
            break;


        case '1': { // FORWARD
            Kitty::kitty().motors.run();

            motorL += 0.1f;
            motorR += 0.1f;

            motorL = std::clamp(motorL, -0.3f, 0.3f);
            motorR = std::clamp(motorR, -0.3f, 0.3f);

            Kitty::kitty().motors.setValue(motorL, motorR);
            break;
        }

        case '2': { // REVERSE
            Kitty::kitty().motors.run();

            motorL -= 0.1f;
            motorR -= 0.1f;

            motorL = std::clamp(motorL, -0.2f, 0.2f);
            motorR = std::clamp(motorR, -0.2f, 0.2f);

            Kitty::kitty().motors.setValue(motorL, motorR);
            break;
        }
        case '3': // right
            Kitty::kitty().servo.set(Kitty::kitty().servo.get() + 0.2f);
            break;
        case '4': // left
            Kitty::kitty().servo.set(Kitty::kitty().servo.get() - 0.2f);
            break;
        case 'x': // reset
            fctprintf(logWrite, NULL, "kittyReset\n");
            NVIC_SystemReset();
            break;
        default: break;
    }
}


void Kitty::uartKLZCallback(uint8_t data) { Kitty::kitty().uartFrame.deserialize(&data, sizeof(data)); }

void Kitty::onKLZDataReceivedCallback(uint8_t* data, size_t length) {
    if (length == 0 || length > 2) {
        return;
    }

    uint16_t distance = 0;
    distance          = static_cast<uint16_t>(data[0] << 8) | data[1];
    fctprintf(logWrite, NULL, "UART_Des KLZ: %d\r\n", distance);

    Kitty::kitty().differential.setKlzDistance(distance);
}

void Kitty::init() {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    FTM_Init();
    uartDebug.init();
    uartDebug.initDMA();
    uartDebug.enableInterrupt(NXP_Uart::InterruptType::RX_FULL);
    uartDebug.setRedirectHandler(uartCallback);
    log_setWriteFunction(logWrite);
    uartCommunication.init();
    uartCommunication.initDMA();

    uartKLZ.init();
    uartKLZ.initDMA();
    uartKLZ.enableInterrupt(NXP_Uart::InterruptType::RX_FULL);
    uartKLZ.setRedirectHandler(uartKLZCallback);

    ledLine.init();
    display.init();
    servo.init();
    servo.set(0.0);
    servo.disable();
    camera.init();
    pitCamera.init();
    pitSendCameraData.init();
    encoderRight.init();
    encoderLeft.init();
    menu.init();
    motors.init();
    motors.setValue(0.2, 0.2);
    // commandManager.init(printCommandManager);

    camera.start();
    pitCamera.setFrequency(params.getCameraPitFrequency());
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t*>(&encoderRight));
    encodersPit.appendCallback(NXP_Encoder::ISR, reinterpret_cast<uint32_t*>(&encoderLeft));
    encodersPit.init();
    // Prepare SR04 PIT for one-shot 10us pulse generation (do not run yet)

    sr04.init();
    // uartCommunication.setRedirectHandler([](uint8_t ch) {Kitty::kitty().commandManager.put_char(ch);});
    display.enable();

    uartCommunication.write("Bejbi don't hurt me", 19);

    menu.init();
    camera.start();
    log_notice("KiTTy init finished");

    // servo.init();
    // servo.set(0.9f);
    // return;

}


void Kitty::FTM_Init() {
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TIMESRC(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
}

static constexpr size_t FRAME_SIZE = 191;
uint8_t frame[FRAME_SIZE]; // 1 start + 128 danych + 1 end

bool menuActive = false;
uint8_t sr04Triggered = 0;

void Kitty::proc() {
    if (!sr04Triggered) {
        magicDiodComposition();
    }

    if (differential.isFinalStopDone()) {
        servo.set(0);
        motors.setValue(0, 0);
        return;
    }

    menuActive = menu.proc(millis());

    uint32_t dist = sr04.getDistanceMm();

    if (cameraDataReceived && !sr04Triggered) {
        cameraDataReceived = false;


        camera.getData(cameraDataBuf);

        float position = newAlgorithm.calculatePosition(cameraDataBuf, millis());

        ////////////////////////////// LOG ////////////////////////////////
        size_t idx   = 0;
        frame[idx++] = 0;   // start
        frame[idx++] = 255; // start
        frame[idx++] = 0;   // start
        frame[idx++] = 255; // start

        uint16_t* buffer = static_cast<uint16_t*>(cameraDataBuf);

        for (size_t i = 0; i < 128; i++) frame[idx++] = (uint8_t)(buffer[i] / 110);

        frame[132] = (uint8_t)(position + 63);

        uint16_t rpmLeft  = encoderLeft.getRPM();
        uint16_t rpmRight = encoderRight.getRPM();

        // Left RPM (2 bajty)
        frame[134] = (rpmLeft >> 8) & 0xFF; // MSB
        frame[135] = rpmLeft & 0xFF;        // LSB

        // Right RPM (2 bajty)
        frame[136] = (rpmRight >> 8) & 0xFF; // MSB
        frame[137] = rpmRight & 0xFF;        // LSB


        uint16_t startRPM = params.getStartRPM();
        frame[138]        = (startRPM >> 8) & 0xFF;
        frame[139]        = startRPM & 0xFF;


        uint16_t sr04Distance = sr04.getDistanceMm();
        frame[140]            = (sr04Distance >> 8) & 0xFF;
        frame[141]            = sr04Distance & 0xFF;


        uint16_t diffLeftValue  = (uint16_t)((differential.getLeft() + 1) * 100);  // -1:1 -> 0:200
        uint16_t diffRightValue = (uint16_t)((differential.getRight() + 1) * 100); // -1:1 -> 0:200
        frame[142]              = (diffLeftValue >> 8) & 0xFF;
        frame[143]              = diffLeftValue & 0xFF;
        frame[144]              = (diffRightValue >> 8) & 0xFF;
        frame[145]              = diffRightValue & 0xFF;
        frame[146]              = menuActive ? 0x01 : 0x00;
        frame[147]              = kitty().newAlgorithm.isPatternDetected() ? 0x01 : 0x00;

        uint16_t servoDivider = (uint16_t)(params.getServoDivider() * 100);
        frame[148] = (servoDivider >> 8) & 0xFF;
        frame[149] = servoDivider & 0xFF;

        uint16_t filterAlpha = (uint16_t)(params.getAlgorithmFilterAlpha() * 1000);
        frame[150] = (filterAlpha >> 8) & 0xFF;
        frame[151] = filterAlpha & 0xFF;

        uint16_t patternTh = (uint16_t)(params.getPatternCorrelationThreshold() * 1000);
        frame[152] = (patternTh >> 8) & 0xFF;
        frame[153] = patternTh & 0xFF;

        uint16_t kp = (uint16_t)(params.getPidKp() * 1000);
        frame[154] = (kp >> 8) & 0xFF;
        frame[155] = kp & 0xFF;

        uint16_t ki = (uint16_t)(params.getPidKi() * 100000);
        frame[156] = (ki >> 8) & 0xFF;
        frame[157] = ki & 0xFF;

        uint16_t brakeAll = (uint16_t)(params.getBrakeAll() * 1000);
        frame[158] = (brakeAll >> 8) & 0xFF;
        frame[159] = brakeAll & 0xFF;

        uint16_t brakeOne = (uint16_t)(params.getBrakeOne() * 1000);
        frame[160] = (brakeOne >> 8) & 0xFF;
        frame[161] = brakeOne & 0xFF;

        uint16_t brakeClamp = (uint16_t)(params.getBrakeClamp() * 1000);
        frame[162] = (brakeClamp >> 8) & 0xFF;
        frame[163] = brakeClamp & 0xFF;

        uint16_t diffClamp = (uint16_t)(params.getDiffClamp() * 1000);
        frame[164] = (diffClamp >> 8) & 0xFF;
        frame[165] = diffClamp & 0xFF;

        uint16_t rpmOffset = params.getRPMOffset();
        frame[166] = (rpmOffset >> 8) & 0xFF;
        frame[167] = rpmOffset & 0xFF;

        uint16_t brightness = static_cast<uint16_t>(newAlgorithm.getBrightness() / 110);
        frame[168] = (brightness >> 8) & 0xFF;
        frame[169] = brightness & 0xFF;

        uint16_t leftLine = static_cast<uint16_t>(newAlgorithm.getLeftLinePosition());
        frame[170] = (leftLine >> 8) & 0xFF;
        frame[171] = leftLine & 0xFF;

        uint16_t rightLine = static_cast<uint16_t>(newAlgorithm.getRightLinePosition());
        frame[172] = (rightLine >> 8) & 0xFF;
        frame[173] = rightLine & 0xFF;

        frame[174] = newAlgorithm.getCrossings();

        uint16_t crossingsBrightnessMultiplier =
            static_cast<uint16_t>(params.getCrossingsBrightnessMultiplier() * 1000.0f);

        frame[175] = (crossingsBrightnessMultiplier >> 8) & 0xFF;
        frame[176] = crossingsBrightnessMultiplier & 0xFF;

        uint16_t brightnessMeanAlpha =
            static_cast<uint16_t>(params.getBrightnessMeanAlpha() * 1000.0f);

        frame[177] = (brightnessMeanAlpha >> 8) & 0xFF;
        frame[178] = brightnessMeanAlpha & 0xFF;

        frame[179] = params.getCrossingsCut();
        
        uint16_t cameraCrossingFlterAlpha =
            static_cast<uint16_t>(params.getCameraCrossingFilterAlpha() * 1000.0f);
        frame[180] = (cameraCrossingFlterAlpha >> 8) & 0xFF;
        frame[181] = cameraCrossingFlterAlpha & 0xFF;

        uint16_t brightnessCrossThreshold = static_cast<uint16_t>(newAlgorithm.getBrightnessCrossThreshold() / 110);
        frame[182] = (brightnessCrossThreshold >> 8) & 0xFF;
        frame[183] = brightnessCrossThreshold & 0xFF;

        uint8_t frequency = static_cast<uint8_t>(params.getCameraPitFrequency() / 1000);
        frame[184] = frequency;

        uint16_t algorithmOffset = static_cast<uint16_t>(params.getAlgorithmOffset() * 1000.0f);
        frame[185] = (algorithmOffset >> 8) & 0xFF;
        frame[186] = algorithmOffset & 0xFF;

        uint16_t patternDetectTimeout = params.getPatternDetectTimeoutMs();
        frame[187] = (patternDetectTimeout >> 8) & 0xFF;
        frame[188] = patternDetectTimeout & 0xFF;

        uint16_t kd = (uint16_t)(params.getPidKd() * 1000);
        frame[189] = (kd >> 8) & 0xFF;
        frame[190] = kd & 0xFF;


        // if(newAlgorithm.isPatternDetected()) {
        //     uartDebug.write((char*)frame, FRAME_SIZE);
        // }

        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter % 2 == 0) {
            uartDebug.write((char*)frame, FRAME_SIZE);
        }



        if (menuActive) 
            return;
        
        
        float servoPosition = -(position / params.getServoDivider());
        if (differential.isDistanceModeActive()) servoPosition = 0;

        servo.set(servoPosition);
        differential.proc(position, millis(), dist);
        motors.setValue(differential.getLeft(), differential.getRight());
    }
}

void Kitty::magicDiodComposition() {
    static uint32_t licznik   = 0;
    static int8_t   ledIndex  = 0;
    static uint8_t  direction = 0;
    static uint8_t  oldLed    = 0;
    licznik++;
    if (licznik == 5000) {
        licznik = 0;
        if (direction == 0) {
            oldLed = ledIndex;
            ledIndex++;
            if (ledIndex == 8) {
                ledIndex  = 6;
                direction = 1;
                oldLed    = 7;
            }
            ledLine.at(ledIndex).set();
            ledLine.at(oldLed).reset();
        } else if (direction == 1) {
            oldLed = ledIndex;
            ledIndex--;
            if (ledIndex == -1) {
                ledIndex  = 1;
                direction = 0;
                oldLed    = 0;
            }
            ledLine.at(ledIndex).set();
            ledLine.at(oldLed).reset();
        }
    }
}