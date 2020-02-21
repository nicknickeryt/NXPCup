set (CWARN "-Wall -Wstrict-prototypes -Wextra ")
set (CXXWARN "-Wall -Wextra")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
#set (ARMFLOAT "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMCU "-mcpu=cortex-m0plus -mthumb ${ARMFLOAT}")

# TODO MKV58
set(RANDOM_DEFS "-DCPU_MKL25Z128xxx4")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -Dregister=\"/**/\" -fconcepts -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/MKL25Z128xxx4_Project_Release.ld)

set(PROJECT_DIRECTORIES "board CMSIS drivers source startup NXP_hal debug_module")


