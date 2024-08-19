set(NAME MKV)

set(DEVICE "CPU_MKV58F1xxx4")
add_compile_definitions(	CPU_MKV58F1M0VLQ24 
							CPU_MKV58F1M0VLQ24_cm7
							CPU_MKV58F1xxx4
							FSL_RTOS_BM
							SDK_OS_BAREMETAL
						)

include(${CMAKE_CURRENT_LIST_DIR}/board/board.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMSIS/CMSIS.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/debug_module/debug_module.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/drivers/drivers.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MKV_HAL/MKV_hal.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake)

set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/source/cpp_config.cpp
			# ${CMAKE_CURRENT_LIST_DIR}/source/commandManager.cpp
			${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
			${CMAKE_CURRENT_LIST_DIR}/source/semihost_hardfault.c)

add_executable(${NAME} ${SOURCES})
include_directories(${CMAKE_CURRENT_LIST_DIR}/source)

set (CWARN "-Wall -Wstrict-prototypes -Wextra ")
set (CXXWARN "-Wall -Wextra")
set (CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
#set (ARMFLOAT "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
set (CMCU "-mcpu=cortex-m0plus -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -fconcepts -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/source/cpp_config.cpp PROPERTIES COMPILE_FLAGS "-w")

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/MKV58F1M0xxx24.ld)
set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} ${ARMFLOAT} -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME} halina board CMSIS debug_module drivers MKV_hal)


add_custom_command(TARGET ${NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O ihex "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.hex"
        COMMAND ${CMAKE_OBJCOPY} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.elf")

target_stlink_flash(${NAME})