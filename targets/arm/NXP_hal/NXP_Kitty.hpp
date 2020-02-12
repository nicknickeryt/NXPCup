/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#pragma once

#include "HALina.hpp"
#include "NXP_gpio.hpp"
#include "NXP_uart.hpp"
#include "NXP_display.hpp"
#include "NXP_servo.hpp"
#include "NXP_motor.hpp"
#include "NXP_PIT.hpp"
#include "NXP_adc.hpp"
#include "NXP_camera.hpp"
#include "NXP_DMA.h"
#include "NXP_encoder.hpp"
#include "commandManager.h"
#include "command_terminal/command_manager.h"
#include "algorithm_unit.hpp"

void pit_generalHandler(uint32_t*);

class Kitty{
private:
    // LEDS
    NXP_GPIO LED0 = NXP_GPIO(PORTA, GPIOA, 16U);
    NXP_GPIO LED1 = NXP_GPIO(PORTA, GPIOA, 17U);
    NXP_GPIO LED2 = NXP_GPIO(PORTA, GPIOA, 24U);
    NXP_GPIO LED3 = NXP_GPIO(PORTA, GPIOA, 25U);
    NXP_GPIO LED4 = NXP_GPIO(PORTA, GPIOA, 26U);
    NXP_GPIO LED5 = NXP_GPIO(PORTA, GPIOA, 27U);
    NXP_GPIO LED6 = NXP_GPIO(PORTA, GPIOA, 28U);
    NXP_GPIO LED7 = NXP_GPIO(PORTA, GPIOA, 29U);

    // ENCODERS
    NXP_PORT encoderRightA = {PORTB, 18, 0x06};
    NXP_PORT encoderRightB = {PORTB, 19, 0x06};
    NXP_PORT encoderLeftA = {PORTA, 12, 0x07};
    NXP_PORT encoderLeftB = {PORTA, 13, 0x07};

    // MOTORS
    NXP_GPIO motorEnablePin = NXP_GPIO(PORTE, GPIOE, 4U);
    NXP_PORT motorLeftPortMLB = {PORTE, 5, 0x06};
    NXP_PORT motorLeftPortMLF = {PORTE, 6, 0x06};
    NXP_PORT motorRightPortMLB = {PORTE, 7, 0x06};
    NXP_PORT motorRightPortMLF = {PORTE, 8, 0x06};
    NXP_PWM motorLeftPwm = {FTM3, motorLeftPortMLB, motorLeftPortMLF, 0, 1, 200};
    NXP_PWM motorRightPwm = {FTM3, motorRightPortMLB, motorRightPortMLF, 2, 3, 200};
    NXP_Motor motorLeft = {motorLeftPwm, motorEnablePin};
    NXP_Motor motorRight = {motorRightPwm, motorEnablePin};

    // UARTS
    NXP_PORT uart0RXmux = {PORTA, 14U, 0x03};
    NXP_PORT uart0TXmux = {PORTA, 15U, 0x03};
    NXP_PORT uart2RXmux = {PORTE, 17U, 0x03};
    NXP_PORT uart2TXmux = {PORTE, 16U, 0x03};

    // SERVO
    NXP_PORT servoPort = {PORTA, 7, 0x03};
    NXP_PWM servoPwm = {FTM0, servoPort, NXP_PORT::getEmptyPort(), 4, 0, 200};

    // CAMERA
    NXP_GPIO cameraClockPin = {PORTB, GPIOB, 6, halina::GPIO::Mode::OUTPUT};
    NXP_GPIO cameraSIPin = {PORTB, GPIOB, 5, halina::GPIO::Mode::OUTPUT};
    NXP_PORT adc0mux = {PORTB, 0U, 0x00};
    NXP_PORT adc1mux = {PORTB, 1U, 0x00};

    // ADC
    NXP_ADC adc = {HSADC0, nullptr, NXP_Camera::adcInterruptEndOfMeasurementStatic};
    NXP_ADC::Sample camera2Sample = {adc0mux, NXP_ADC::ChannelSingleEnded::B_CH2};
    NXP_ADC::Sample camera1Sample = {adc1mux, NXP_ADC::ChannelSingleEnded::B_CH3};

    // DMA
    NXP_DMA uart0DMA = {kDmaRequestMux0UART0Tx};

    // PIT
    NXP_PIT pitCamera = {NXP_PIT::CHANNEL::_0, 55000, NXP_Camera::pitInterruptStatic, nullptr};
    NXP_PIT pitSendCameraData = {NXP_PIT::CHANNEL::_1, 19, pit_generalHandler, nullptr};
    NXP_PIT encodersPit = {NXP_PIT::CHANNEL::_2, 100, nullptr, nullptr};

    // COMMAND TERMINAL
    CommandManager<3, '\n', false> commandManager{__enable_irq, __disable_irq,
                                                    {
                                                            Command("m", "", motorsCallback),
                                                            Command("s", "", servoCallback),
                                                            Command("st", "", stopCallback)
                                                    }};

public:
    // ENKODER
    NXP_Encoder encoderLeft = {FTM1, encoderLeftA, encoderLeftB, NXP_Encoder::Mode::SingleCounter};
    NXP_Encoder encoderRight = {FTM2, encoderRightA, encoderRightB, NXP_Encoder::Mode::SingleCounter};

    // KAMERA
    NXP_Camera camera = {NXP_Camera::Type::BOTH, adc, cameraClockPin, cameraSIPin, camera1Sample, camera2Sample, uartCommunication};

    // UART
    NXP_Uart uartDebug = {UART2, 115200, uart2RXmux, uart2TXmux, NXP_DMA::emptyDMA()};
    NXP_Uart uartCommunication = {UART0, 115200, uart0RXmux, uart0TXmux, uart0DMA};

    // LEDS
    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};
    // DISPLAY
    NXP_Display display;
    // SERVO
    NXP_Servo servo = {servoPwm, 70, 2.0f};
    // MOTORS
    NXP_Motors motors = {motorLeft, motorRight};
    // ALGORITHM
    AlgorithmUnit algorithmUnit = {uartCommunication};

private:
    Kitty() = default;

    static void FTM_Init();

    void magicDiodComposition();

public:
    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }

    void init();

    void proc();

    static void printCommandManager(char data){
        Kitty::kitty().uartCommunication.write(data);
    }
};