/**
 * Copyright (c) Kolo Naukowe Elektronikow, Akademia Gorniczo-Hutnicza im. Stanislawa Staszica w Krakowie 2020
 * Authors: Arkadiusz Balys, Kamil Kasperczyk, Witold Lukasik
 *
 * Main Kitty class
 *
 */

#pragma once

#include "HALina.hpp"
#include "NXP_DMA.h"
#include "NXP_I2C.hpp"
#include "NXP_PIT.hpp"
#include "NXP_adc.hpp"
#include "NXP_camera.hpp"
#include "NXP_display.hpp"
#include "NXP_encoder.hpp"
#include "NXP_frame.hpp"
#include "NXP_gpio.hpp"
#include "NXP_menu.hpp"
#include "NXP_motor.hpp"
#include "NXP_servo.hpp"
#include "NXP_uart.hpp"
#include "algorithm.hpp"

void pit_generalHandler(uint32_t*);

void pit_sendCameraData(uint8_t);

void logWrite(char c,[[maybe_unused]] void* const context);

class Kitty {
  private:
    // SYSTICK
    static uint_fast64_t milliseconds;

    // LEDS
    NXP_GPIO LED0 = NXP_GPIO(PORTA, GPIOA, 16U);
    NXP_GPIO LED1 = NXP_GPIO(PORTA, GPIOA, 17U);
    NXP_GPIO LED2 = NXP_GPIO(PORTA, GPIOA, 24U);
    NXP_GPIO LED3 = NXP_GPIO(PORTA, GPIOA, 25U);
    NXP_GPIO LED4 = NXP_GPIO(PORTA, GPIOA, 26U);
    NXP_GPIO LED5 = NXP_GPIO(PORTA, GPIOA, 27U);
    NXP_GPIO LED6 = NXP_GPIO(PORTA, GPIOA, 28U);
    NXP_GPIO LED7 = NXP_GPIO(PORTA, GPIOA, 29U);

    // SWITCHES
    NXP_GPIO         SWITCH0  = {PORTB, GPIOB, 20U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO         SWITCH1  = {PORTB, GPIOB, 21U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO         SWITCH2  = {PORTB, GPIOB, 22U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO         SWITCH3  = {PORTB, GPIOB, 23U, NXP_GPIO::Mode::INPUT};
    halina::Switches switches = {SWITCH0, SWITCH1, SWITCH2, SWITCH3};

    // BUTTONS
    NXP_GPIO        BUTTON0 = {PORTC, GPIOC, 16U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO        BUTTON1 = {PORTC, GPIOC, 17U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO        BUTTON2 = {PORTC, GPIOC, 18U, NXP_GPIO::Mode::INPUT};
    NXP_GPIO        BUTTON3 = {PORTC, GPIOC, 19U, NXP_GPIO::Mode::INPUT};
    halina::Buttons buttons = {BUTTON0, BUTTON1, BUTTON2, BUTTON3};

    // LEDS
    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};

    // ENCODERS
    NXP_PORT encoderRightA = {PORTB, 18, 0x06};
    NXP_PORT encoderRightB = {PORTB, 19, 0x06};
    NXP_PORT encoderLeftA  = {PORTA, 12, 0x07};
    NXP_PORT encoderLeftB  = {PORTA, 13, 0x07};

    // MOTORS
    NXP_GPIO  motorEnablePin        = NXP_GPIO(PORTE, GPIOE, 4U);
    NXP_PORT  motorLeftPortMLB      = {PORTE, 5, 0x06};
    NXP_PORT  motorLeftPortMLF      = {PORTE, 6, 0x06};
    NXP_PORT  motorRightPortMLB     = {PORTE, 7, 0x06};
    NXP_PORT  motorRightPortMLF     = {PORTE, 8, 0x06};
    NXP_PWM   motorLeftForwardPwm   = {FTM3, motorLeftPortMLF, 0, 200};
    NXP_PWM   motorLeftBackwardPwm  = {FTM3, motorLeftPortMLB, 1, 200};
    NXP_PWM   motorRightForwardPwm  = {FTM3, motorRightPortMLF, 2, 200};
    NXP_PWM   motorRightBackwardPwm = {FTM3, motorRightPortMLB, 3, 200};
    NXP_Motor motorLeft             = {motorLeftForwardPwm, motorLeftBackwardPwm, motorEnablePin};
    NXP_Motor motorRight            = {motorRightForwardPwm, motorRightBackwardPwm, motorEnablePin};

    // UARTS
    NXP_PORT uart0RXmux = {PORTA, 14U, 0x03};
    NXP_PORT uart0TXmux = {PORTA, 15U, 0x03};
    NXP_PORT uart1RXmux = {PORTC, 3U, 0x03};
    NXP_PORT uart1TXmux = {PORTC, 4U, 0x03};
    NXP_PORT uart2RXmux = {PORTE, 17U, 0x03};
    NXP_PORT uart2TXmux = {PORTE, 16U, 0x03};
    NXP_PORT uart4TXmux = {PORTE, 25U, 0x08};
    NXP_PORT uart4RXmux = {PORTE, 24U, 0x08};

    // SERVO
    NXP_PORT servoPort = {PORTA, 7, 0x03};
    NXP_PWM  servoPwm  = {FTM0, servoPort, 4, 200};

    // CAMERA
    NXP_GPIO cameraClockPin = {PORTB, GPIOB, 6, halina::GPIO::Mode::OUTPUT};
    NXP_GPIO cameraSIPin    = {PORTB, GPIOB, 5, halina::GPIO::Mode::OUTPUT};
    NXP_PORT adc0mux        = {PORTB, 0U, 0x00};
    NXP_PORT adc1mux        = {PORTB, 1U, 0x00};

    // ADC
    NXP_ADC         adc           = {HSADC0, nullptr, NXP_Camera::adcInterruptEndOfMeasurementStatic};
    NXP_ADC::Sample camera2Sample = {adc0mux, NXP_ADC::ChannelSingleEnded::B_CH2};
    NXP_ADC::Sample camera1Sample = {adc1mux, NXP_ADC::ChannelSingleEnded::B_CH3};

    // DMA
    NXP_DMA uart0DMA = {kDmaRequestMux0UART0Tx};

    // PIT
    NXP_PIT pitCamera         = {NXP_PIT::CHANNEL::_0, 25000, NXP_Camera::pitInterruptStatic, nullptr};
    NXP_PIT pitSendCameraData = {NXP_PIT::CHANNEL::_1, 30, pit_generalHandler, nullptr};
    NXP_PIT encodersPit       = {NXP_PIT::CHANNEL::_2, 50, nullptr, nullptr};

    // // COMMAND TERMINAL
    // CommandManager<3, '\n', false> commandManager{__enable_irq, __disable_irq,
    //                                                 {
    //                                                         Command("m", "", motorsCallback),
    //                                                         Command("s", "", servoCallback),
    //                                                         Command("st", "", stopCallback)
    //                                                 }};

    // I2C
    NXP_PORT sdaPort = {PORTE, 0, 6, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_PORT sclPort = {PORTE, 1, 6, NXP_PORT::Pull::PullUp, NXP_PORT::OpenDrain::Enable};
    NXP_I2C  i2c     = {I2C1, sdaPort, sclPort, 400000};

    // FRAME
    NXP_Frame frame = {uartCommunication};

  public:
    // ENKODER
    NXP_Encoder encoderLeft  = {FTM1, encoderLeftA, encoderLeftB, NXP_Encoder::Mode::SingleCounter};
    NXP_Encoder encoderRight = {FTM2, encoderRightA, encoderRightB, NXP_Encoder::Mode::SingleCounter};

    // KAMERA
    NXP_Camera::Type cameraType   = NXP_Camera::Type::CAMERA_1;
    NXP_Camera       camera       = {cameraType, adc, cameraClockPin, cameraSIPin, camera1Sample, camera2Sample, uartDebug};
    uint16_t         cameraDataBuf[128] = {0};

    // UART
    NXP_Uart uartCommunication = {UART2, 115200, uart2RXmux, uart2TXmux, NXP_DMA::emptyDMA()};
    NXP_Uart uartDebug         = {UART0, 115200, uart0RXmux, uart0TXmux, uart0DMA};

    // DISPLAY
    NXP_Display display;

    // SERVO
    NXP_Servo servo = {0.1, servoPwm, 40, 2.0f};

    // MOTORS
    NXP_Motors motors = {motorLeft, motorRight};

    // ALGORITHMzzzz
    Algorithm     newAlgorithm;

    // MENU
    NXP_Menu menu = {buttons, switches, display, motors};


  private:
    Kitty() = default;

    static void FTM_Init();

    

  public:
    void magicDiodComposition();

    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }

    void init();

    static void millisIncrease() { milliseconds++; }

    static uint32_t millis() { return milliseconds; }

    static void printCommandManager(char data) { Kitty::kitty().uartCommunication.write(data); }
};