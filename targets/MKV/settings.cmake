set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
set (CXXWARN "-Wall -Wextra")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
#set (ARMFLOAT "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMCU "-mcpu=cortex-m7 -mthumb ${ARMFLOAT}")

# TODO MKV58
set(RANDOM_DEFS "-DMKV58F1M0xxx24")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -Dregister=\"/**/\" -fconcepts -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/MKV58F1M0xxx24.ld)

set(PROJECT_DIRECTORIES "board CMSIS device drivers source startup NXP_hal debug_module algorithms NXP_vl53l0x vl53l0x")


