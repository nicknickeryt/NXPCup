#pragma once

#include "static_circular_deque.h"

#ifndef SHARED_MEM_M0
#define SHARED_MEM_M0          0x10089F80
#endif
#ifndef SHARED_MEM_M4
#define SHARED_MEM_M4          0x10089FC0
#endif


struct Event {
    void* functionPointer;
    void* dataPointer;
    uint8_t dataSize;
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
};