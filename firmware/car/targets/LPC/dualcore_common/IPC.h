#pragma once

#include "static_circular_deque.h"

#ifndef SHARED_MEM_M0
#define SHARED_MEM_M0          0x10089F80
#endif
#ifndef SHARED_MEM_M4
#define SHARED_MEM_M4          0x10089FC0
#endif
void setNVIC_M4();
struct Event {
    enum class Type : uint8_t {
        NONE = 0,
        VAR_BOOL,
        VAR_UINT8,
        VAR_UINT16,
        VAR_UINT32,
        BUFFER_UINT8,
        BUFFER_UINT16,
        BUFFER_UINT32,
        FUN
    };
    Type type;
    uint32_t data1;
    uint32_t data2;
    uint8_t data2Size;

    Event(Type type, void* data1, void* data2, uint8_t data2Size)
            : type(type), data1(reinterpret_cast<uint32_t>(data1)), data2(reinterpret_cast<uint32_t>(data2)), data2Size(data2Size) {}

    Event(Type type, uint32_t data1, uint32_t data2, uint8_t data2Size)
            : type(type), data1(data1), data2(data2), data2Size(data2Size) {}

    Event() : type(Type::NONE), data1(0), data2(0), data2Size(0) {}
};

enum class CPU {
    M0,
    M4
};

template <CPU C> class IPC   {
public:
    using deque = static_circular_deque<Event, 100>;
private:
    deque* myDeque;
    deque* hisDeque;

public:
     IPC(uint32_t myDequeAddr, uint32_t hisDequeAddr) {
         if constexpr (CPU::M4 == C) {
             myDeque = new((void*) myDequeAddr) deque;
             hisDeque = new((void*) hisDequeAddr) deque;
         } else {
             myDeque = reinterpret_cast<decltype(myDeque)>(myDequeAddr);
             hisDeque = reinterpret_cast<decltype(hisDeque)>(hisDequeAddr);
         }
     }

    ~IPC() {
         if constexpr (CPU::M4 == C) {
             if (myDeque) { myDeque->~deque(); }
             if (hisDeque) { hisDeque->~deque(); }
         }
    }

     void push(uint8_t data) {
         myDeque->push_back( Event(Event::Type::VAR_UINT8, data, 0, 0));
     }

     void setNVIC() ;
};

template <> void IPC<CPU::M4>::setNVIC();

template <> void IPC<CPU::M0>::setNVIC();