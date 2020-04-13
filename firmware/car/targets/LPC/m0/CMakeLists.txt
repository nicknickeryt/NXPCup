set(NAME m0)

add_definitions(${CORE_M0_DEFINITIONS} -D__LPC43XX__  -D__MULTICORE_M0APP -DCORE_M0AP -DCHIP_LPC43XX -DLPC_CREG)
set(CORE_M0_FLAGS "-g3 -Wall -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -fstack-usage")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CORE_M0_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORE_M0_FLAGS}")

file(GLOB_RECURSE SOURCES
        "*.h"
        "*.c"
        "*.cpp"
        "*.hpp"
        "../dualcore_common/*M0.cpp"
        "../dualcore_common/*M0.c"
        "../dualcore_common/*MB.cpp"
        "../dualcore_common/*MB.c"
        "../dualcore_common/*.hpp"
        "../dualcore_common/*.h"
        )

message("SOURCES: ${SOURCES}")

# without warnings
foreach(file ${SOURCES})
    set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(file)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dualcore_common/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_board_ngx_xplorer_4330_m0/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx_m0/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx_m0/inc/config_43xx_m0app/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../dualcore_common/)

set_target_properties(${NAME} PROPERTIES LINK_FLAGS
        "-T ${CMAKE_CURRENT_SOURCE_DIR}/m0_Debug.ld ${CORE_M0_FLAGS} -Xlinker -Map=dualcore_blinky_m0.map -Xlinker -print-memory-usage -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME}
        lpc_board_ngx_xplorer_4330_m0
        lpc_chip_43xx_m0
        core
        )

add_custom_command(TARGET ${NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.elf
        COMMAND arm-none-eabi-objcopy
        --target elf32-littlearm
        --verbose
        --strip-all
        --remove-section=.ARM.attributes
        --redefine-sym __vectors_start__=__vectors_start___core_m0app
        --keep-symbol __vectors_start___core_m0app
        --redefine-sym _data=__start_data_core_m0app
        --keep-symbol __start_data_core_m0app
        --redefine-sym _edata=__end_data_core_m0app
        --keep-symbol __end_data_core_m0app
        --remove-section=".bss*"
        --remove-section=".noinit*"
        --rename-section .ARM.exidx=".core_m0app.ARM.exidx"
        --rename-section .ARM.extab=".core_m0app.ARM.extab"
        --rename-section .text=".core_m0app"
        --rename-section .data=".core_m0app.data"
        --rename-section .data_RAM2=".core_m0app.data_RAM2"
        --rename-section .data_RAM3=".core_m0app.data_RAM3"
        --rename-section .data_RAM4=".core_m0app.data_RAM4"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.o")

