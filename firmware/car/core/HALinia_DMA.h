#pragma once


class HALina_DMA {
    inline static uint32_t usedChannels = 0;
public:
    static int getDMAChannel(dma_request_source_t source) {
        // page 471 reference manual
        if (source & 0x0100u) { // get from first group
            for (uint8_t i=0; i <= 15; i++) {
                if (!(usedChannels & (1u << i))) {
                    usedChannels |= (1u << i);
                    return i;
                }
                return -1;
            }
        } else if (source & 0x0200u) { // get from second group
            for (uint8_t i=16; i <= 31; i++) {
                if (!(usedChannels & (1u << i))) {
                    usedChannels |= (1u << i);
                    return i;
                }
                return -1;
            }
        } else {
            return -1;
        }
        return -1;
    }
};