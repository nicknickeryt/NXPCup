#pragma once

#include "HALina.hpp"
#include "cyclicBuffer.hpp"

namespace halina {

    template<int rx_size, int tx_size>
    class UART {
        CyclicBuffer_data<uint8_t, rx_size> rx_buffer;
        CyclicBuffer_data<uint8_t, tx_size> tx_buffer;
    public:
        UART() {
        }

        virtual void init() = 0;

        virtual void write(void const*) = 0;

        virtual char read() = 0;
    };

}