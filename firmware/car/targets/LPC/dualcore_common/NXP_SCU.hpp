#pragma once

#include <cstdint>
#include "chip.h"


class NXP_SCU {
public:
    enum class Function : uint8_t {
        _0            = 0x0u,				/*!< Selects pin function 0 */
        _1            = 0x1u,				/*!< Selects pin function 1 */
        _2            = 0x2u,				/*!< Selects pin function 2 */
        _3            = 0x3u,				/*!< Selects pin function 3 */
        _4            = 0x4u,				/*!< Selects pin function 4 */
        _5            = 0x5u,				/*!< Selects pin function 5 */
        _6            = 0x6u,				/*!< Selects pin function 6 */
        _7            = 0x7u,				/*!< Selects pin function 7 */
    };

    enum Mode : uint16_t {
        Mode_PULLUP            = (0x0u << 3u),		/*!< Enable pull-up resistor at pad */
        Mode_REPEATER          = (0x1u << 3u),		/*!< Enable pull-down and pull-up resistor at resistor at pad (repeater mode) */
        Mode_INACT             = (0x2u << 3u),		/*!< Disable pull-down and pull-up resistor at resistor at pad */
        Mode_PULLDOWN          = (0x3u << 3u),		/*!< Enable pull-down resistor at pad */
        Mode_HIGHSPEEDSLEW_EN  = (0x1u << 5u),		/*!< Enable high-speed slew */
        Mode_INBUFF_EN         = (0x1u << 6u),		/*!< Enable Input buffer */
        Mode_ZIF_DIS           = (0x1u << 7u),		/*!< Disable input glitch filter */
        Mode__4MA_DRIVESTR     = (0x0u << 8u),		/*!< Normal drive: 4mA drive strength */
        Mode__8MA_DRIVESTR     = (0x1u << 8u),		/*!< Medium drive: 8mA drive strength */
        Mode__14MA_DRIVESTR    = (0x2u << 8u),		/*!< High drive: 14mA drive strength */
        Mode__20MA_DRIVESTR    = (0x3u << 8u),		/*!< Ultra high- drive: 20mA drive strength */
        Mode_FAST              = (SCU_MODE_INACT | SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS)
    };

    enum class Type : uint8_t {
        PIN = 0,
        CLK_OUT = 1,
        I2C = 5,
        ADC = 6,
        DAC = 7
    };

    uint8_t port_or_clk;
    uint8_t pin;
    uint16_t mode;
    Type type;

    NXP_SCU (Type type, Function function, uint16_t mode, uint8_t port_or_clk = 0, uint8_t pin = 0) : type(type), port_or_clk(port_or_clk), pin(pin), mode(static_cast<uint16_t >(function) | mode){

    }

    void init() {
        if (type == Type::PIN) {
            Chip_SCU_PinMuxSet(port_or_clk, pin, mode);
        } else if (type == Type::CLK_OUT) {
            Chip_SCU_ClockPinMuxSet(port_or_clk, mode);
        }

    }

    static void enable (Type type, Function function, uint16_t mode, uint8_t port_or_clk = 0, uint8_t pin = 0) {
        mode  = mode | static_cast<uint16_t >(function);
        if (type == Type::PIN) {
            Chip_SCU_PinMuxSet(port_or_clk, pin, mode);
        } else if (type == Type::CLK_OUT) {
            Chip_SCU_ClockPinMuxSet(port_or_clk, mode);
        }
    }
};