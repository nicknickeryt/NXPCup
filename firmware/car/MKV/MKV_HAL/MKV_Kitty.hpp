#pragma once

#include "HALina_led_line.hpp"
#include "MKV_DMA.h"
#include "MKV_PIT.hpp"
#include "MKV_adc.hpp"
#include "MKV_camera.hpp"
#include "MKV_display.hpp"
#include "MKV_frame.hpp"
#include "MKV_gpio.hpp"
#include "MKV_motor.hpp"
#include "MKV_servo.hpp"
#include "MKV_uart.hpp"

void pit_generalHandler(uint32_t*);

void pit_sendCameraData(uint8_t);

class Kitty {
  private:
    // LEDS
    MKV_GPIO LED0 = MKV_GPIO(PORTA, GPIOA, 29U);
    MKV_GPIO LED1 = MKV_GPIO(PORTA, GPIOA, 28U);
    MKV_GPIO LED2 = MKV_GPIO(PORTA, GPIOA, 27U);
    MKV_GPIO LED3 = MKV_GPIO(PORTA, GPIOA, 26U);
    MKV_GPIO LED4 = MKV_GPIO(PORTA, GPIOA, 25U);
    MKV_GPIO LED5 = MKV_GPIO(PORTA, GPIOA, 24U);
    MKV_GPIO LED6 = MKV_GPIO(PORTA, GPIOA, 17U);
    MKV_GPIO LED7 = MKV_GPIO(PORTA, GPIOA, 16U);

    halina::LedLine ledLine = {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7};

    // SWITCHES
    MKV_GPIO SWITCH0 = {PORTB, GPIOB, 20U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH1 = {PORTB, GPIOB, 21U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH2 = {PORTB, GPIOB, 22U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO SWITCH3 = {PORTB, GPIOB, 23U, MKV_GPIO::Mode::INPUT};

    halina::Switches switches = {SWITCH0, SWITCH1, SWITCH2, SWITCH3};

    // BUTTONS
    MKV_GPIO        BUTTON0 = {PORTC, GPIOC, 16U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO        BUTTON1 = {PORTC, GPIOC, 17U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO        BUTTON2 = {PORTC, GPIOC, 18U, MKV_GPIO::Mode::INPUT};
    MKV_GPIO        BUTTON3 = {PORTC, GPIOC, 19U, MKV_GPIO::Mode::INPUT};
    halina::Buttons buttons = {BUTTON0, BUTTON1, BUTTON2, BUTTON3};

    // MOTORS
    MKV_GPIO  motorEnablePin        = MKV_GPIO(PORTE, GPIOE, 4U);
    MKV_PORT  motorLeftPortMLB      = {PORTE, 5, 0x06};
    MKV_PORT  motorLeftPortMLF      = {PORTE, 6, 0x06};
    MKV_PORT  motorRightPortMLB     = {PORTE, 7, 0x06};
    MKV_PORT  motorRightPortMLF     = {PORTE, 8, 0x06};
    MKV_PWM   motorLeftForwardPwm   = {FTM3, motorLeftPortMLF, 0, 200};
    MKV_PWM   motorLeftBackwardPwm  = {FTM3, motorLeftPortMLB, 1, 200};
    MKV_PWM   motorRightForwardPwm  = {FTM3, motorRightPortMLF, 2, 200};
    MKV_PWM   motorRightBackwardPwm = {FTM3, motorRightPortMLB, 3, 200};
    MKV_Motor motorLeft             = {motorLeftForwardPwm, motorLeftBackwardPwm, motorEnablePin};
    MKV_Motor motorRight            = {motorRightForwardPwm, motorRightBackwardPwm, motorEnablePin};

    // UARTS
    MKV_PORT uart0RXmux = {PORTA, 14U, 0x03};
    MKV_PORT uart0TXmux = {PORTA, 15U, 0x03};
    MKV_PORT uart1RXmux = {PORTC, 3U, 0x03};
    MKV_PORT uart1TXmux = {PORTC, 4U, 0x03};
    MKV_PORT uart2RXmux = {PORTE, 17U, 0x03};
    MKV_PORT uart2TXmux = {PORTE, 16U, 0x03};
    MKV_PORT uart4TXmux = {PORTE, 25U, 0x08};
    MKV_PORT uart4RXmux = {PORTE, 24U, 0x08};

    // DMA
    MKV_DMA uart0DMA = {kDmaRequestMux0UART0Tx};

    // PIT
    MKV_PIT pitCamera         = {MKV_PIT::CHANNEL::_0, 25000, MKV_Camera::pitInterruptStatic, nullptr};
    MKV_PIT pitSendCameraData = {MKV_PIT::CHANNEL::_1, 30, pit_generalHandler, nullptr};
    MKV_PIT encodersPit       = {MKV_PIT::CHANNEL::_2, 50, nullptr, nullptr};

    // SERVO
    MKV_PORT servoPort = {PORTA, 7, 0x03};
    MKV_PWM  servoPwm  = {FTM0, servoPort, 4, 200};

    // CAMERA
    MKV_GPIO cameraClockPin = {PORTB, GPIOB, 6, halina::GPIO::Mode::OUTPUT};
    MKV_GPIO cameraSIPin    = {PORTB, GPIOB, 5, halina::GPIO::Mode::OUTPUT};
    MKV_PORT adc0mux        = {PORTB, 0U, 0x00};
    MKV_PORT adc1mux        = {PORTB, 1U, 0x00};

    // ADC
    MKV_ADC         adc           = {HSADC0, nullptr, MKV_Camera::adcInterruptEndOfMeasurementStatic};
    MKV_ADC::Sample camera2Sample = {adc0mux, MKV_ADC::ChannelSingleEnded::B_CH2};
    MKV_ADC::Sample camera1Sample = {adc1mux, MKV_ADC::ChannelSingleEnded::B_CH3};

  public:
    // DISPLAY
    MKV_Display display;

    // SERVO         offset, pwm, maxDegDeviaton, servoMultiplier
    MKV_Servo servo = {0.1, servoPwm, 40, 2.0f};

    // MOTORS
    MKV_Motors motors = {motorLeft, motorRight};

    // CAMERA
    MKV_Camera camera = {MKV_Camera::Type::BOTH, adc, cameraClockPin, cameraSIPin, camera1Sample, camera2Sample, uartDebug};

    // UART
    MKV_Uart uartCommunication = {UART2, 115200, uart2RXmux, uart2TXmux, MKV_DMA::emptyDMA()};
    MKV_Uart uartDebug         = {UART0, 115200, uart0RXmux, uart0TXmux, uart0DMA};
    MKV_Uart uartToKLZ         = {UART4, 115200, uart4RXmux, uart4TXmux, MKV_DMA::emptyDMA()};

  private:
    Kitty() = default;

  public:
    void init();

    static Kitty& kitty() {
        static Kitty staticKitty;
        return staticKitty;
    }
};