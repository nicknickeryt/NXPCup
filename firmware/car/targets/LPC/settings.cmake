set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
set (CXXWARN "-Wall -Wextra")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
#set (ARMFLOAT "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMCU "-mcpu=cortex-m7 -mthumb ${ARMFLOAT}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -Dregister=\"/**/\" -fconcepts -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")