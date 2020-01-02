set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
set (CXXWARN "-Wall -Wextra -Werror")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")

set(CMCU "-mcpu=cortex-m7 -mthumb -mfloat-abi=soft")

# TODO MKV58
set(RANDOM_DEFS "-DMKV58F1M0xxx24")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -Dregister=\"/**/\" -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/MKV58F1M0xxx24.ld)

set(PROJECT_DIRECTORIES "board CMSIS device drivers source startup NXP_hal")
