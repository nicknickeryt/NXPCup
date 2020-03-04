//
// Created by Kamil on 03.01.2020.
//

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** @addtogroup DRIVERS
 * @{
 */

/** @defgroup RING_BUFFER This module delivers a basic, general-purpose ring buffer.
 * @{
 */

/** Structure describing the ring buffer. */
typedef struct {
    /// pointer to the area of memory, where data will be stored
    uint8_t *dataBuffer;
    /// size of the data buffer (in bytes)
    size_t dataBufferSize;
    /// number of elements currently stored within the buffer
    size_t count;
    /// pointer to the next element in the buffer that will be written
    uint8_t *head;
    /// pointer to the next element in the buffer that will be read
    uint8_t *tail;
} RingBuffer;

/**
 * Initializes the given ring buffer structure.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @param dataBuffer pointer to a location in memory, where the ring buffer data will be stored
 * @param dataBufferSize size in bytes of the dataBuffer
 * @return true if all arguments are valid and the ring buffer is initialized successfully, false otherwise
*/
bool RingBuffer_Init(RingBuffer *ringBuffer, uint8_t *dataBuffer, size_t dataBufferSize);

/**
 * Clears contents of the given ring buffer.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @return true if the ring buffer is cleared successfully, false otherwise
*/
bool RingBuffer_Clear(RingBuffer *ringBuffer);

/**
 * Checks if the given ring buffer is empty.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @return true if the ring buffer holds no data, false otherwise
*/
bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer);

/**
 * Gets the length (in bytes) of the data stored in the given ring buffer.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @return length (in bytes) of the data stored in the ring buffer
*/
size_t RingBuffer_GetLen(const RingBuffer *ringBuffer);

/**
 * Returns the capacity (in bytes) of the given buffer.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @return capacity (in bytes) of the ring buffer (how much characters can it store)
*/
size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer);

/**
 * Returns the space (in bytes) left in the given buffer.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @return space (in bytes) of the ring buffer (how much characters can be still written to it)
*/
size_t RingBuffer_GetSpace(const RingBuffer *ringBuffer);

/**
 * Appends a single character to the ring buffer. The stored data length will be
 * increased by 1.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @param c character to append
 * @return true if the character was added successfully, false otherwise
*/
bool RingBuffer_PutChar(RingBuffer *ringBuffer, uint8_t c);

/**
 * Pulls out a single character from the ring buffer. The stored data length will be
 * decreased by 1.
 *
 * @param ringBuffer pointer to a \ref RingBuffer structure
 * @param c pointer to a place where character will be stored
 * @return true if the character was pulled out successfully, false otherwise
*/
bool RingBuffer_GetChar(RingBuffer *ringBuffer, uint8_t *c);

/**
 *   @}
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // RING_BUFFER_H

