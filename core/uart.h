#ifndef PROJECT_NAME_UART_H
#define PROJECT_NAME_UART_H

#include <gsl/gsl>
#include "cyclicBuffer.hpp"

template <int rx_size, int tx_size>
class UART {
    CyclicBuffer_data<uint8_t , rx_size>rx_buffer;
    CyclicBuffer_data<uint8_t , tx_size>tx_buffer;
public:
    UART() {

    }
    virtual void write() = 0;
    virtual char read() = 0;
};


#endif //PROJECT_NAME_UART_H
