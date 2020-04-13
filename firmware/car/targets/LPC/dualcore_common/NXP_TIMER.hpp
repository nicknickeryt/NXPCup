#pragma once
#include <cstdint>

class NXP_Timer {
public:
    class Channel {
    public:
        enum class Match : uint8_t {
            INT = 0,
            RESET = 1,
            STOP = 2
        };

        uint8_t number;
        uint8_t match;
        Channel(uint8_t number) : number(number) {
            match = 0;
        }

        void setMatch(Match match) {
            this->match |= (1u << static_cast<uint8_t>(match));
        }

        void init() {

        }

    };

    NXP_Timer(uint32_t frequency) {

    }

    void init() {

    }
};