#include "HALina_ring_buffer_MB.hpp"
#include <cassert>

bool HRingBuffer_Init(RingBuffer *ringBuffer, uint8_t *dataBuffer, size_t dataBufferSize) {
    assert(ringBuffer);
    assert(dataBuffer);
    assert(dataBufferSize > 0);

    if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
        ringBuffer->dataBuffer = dataBuffer;
        ringBuffer->dataBufferSize = dataBufferSize;
        ringBuffer->count = 0;
        ringBuffer->head = dataBuffer;
        ringBuffer->tail = dataBuffer;
        return true;
    }

    return false;
}

bool HRingBuffer_Clear(RingBuffer *ringBuffer) {
    assert(ringBuffer);

    if (ringBuffer) {
        ringBuffer->count = 0;
        ringBuffer->head = ringBuffer->dataBuffer;
        ringBuffer->tail = ringBuffer->dataBuffer;
        return true;
    }
    return false;
}

bool HRingBuffer_IsEmpty(const RingBuffer *ringBuffer) {
    assert(ringBuffer);

    if (ringBuffer) {
        return (ringBuffer->count == 0);
    }

    return true;
}

size_t HRingBuffer_GetLen(const RingBuffer *ringBuffer) {
    assert(ringBuffer);

    if (ringBuffer) {
        return ringBuffer->count;
    }
    return 0;
}

size_t HRingBuffer_GetCapacity(const RingBuffer *ringBuffer) {
    assert(ringBuffer);

    if (ringBuffer) {
        return ringBuffer->dataBufferSize;
    }
    return 0;
}

size_t HRingBuffer_GetSpace(const RingBuffer *ringBuffer) {
    assert(ringBuffer);
    if (ringBuffer) {
        return ringBuffer->dataBufferSize - ringBuffer->count;
    }
    return 0;
}

bool HRingBuffer_PutChar(RingBuffer *ringBuffer, uint8_t c) {
    assert(ringBuffer);

    if ((ringBuffer) && (ringBuffer->count < ringBuffer->dataBufferSize)) {
        *ringBuffer->head = c;
        ringBuffer->count++;
        ringBuffer->head++;
        if (ringBuffer->head >= ringBuffer->dataBuffer + ringBuffer->dataBufferSize) {
            ringBuffer->head = ringBuffer->dataBuffer;
        }
        return true;
    }
    return false;
}

bool HRingBuffer_GetChar(RingBuffer *ringBuffer, uint8_t *c) {
    assert(ringBuffer);
    assert(c);

    if ((ringBuffer) && (c) && (ringBuffer->count)) {
        *c = *ringBuffer->tail;
        ringBuffer->count--;
        ringBuffer->tail++;
        if (ringBuffer->tail >= ringBuffer->dataBuffer + ringBuffer->dataBufferSize) {
            ringBuffer->tail = ringBuffer->dataBuffer;
        }
        return true;
    }
    return false;
}
