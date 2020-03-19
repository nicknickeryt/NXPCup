set(NAME m4)

add_definitions(${CORE_M4_DEFINITIONS} -D__LPC43XX__ -DLPC43_MULTICORE_M0APP -D__MULTICORE_MASTER -D__MULTICORE_MASTER_SLAVE_M0APP -DCHIP_LPC43XX -DLPC_CREG)
set(CORE_M4_FLAGS "-fno-common -g3 -Wall -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CORE_M4_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORE_M4_FLAGS}")

file(GLOB_RECURSE SOURCES
        "*.h"
        "*.c"
        "*.cpp"
        "*.hpp"
        "../dualcore_common/*M4.cpp"
        "../dualcore_common/*.h"
        )

message("SOURCES: ${SOURCES}")

# without warnings
foreach(file ${SOURCES})
    set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(file)

SET(M0_OBJ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/m0.o )

SET_SOURCE_FILES_PROPERTIES( ${M0_OBJ}  PROPERTIES  EXTERNAL_OBJECT true  GENERATED true )

add_executable(${NAME} ${SOURCES}  ${M0_OBJ} )
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dualcore_common/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_board_ngx_xplorer_4330/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/config_43xx/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../dualcore_common/)

set_target_properties(${NAME} PROPERTIES LINK_FLAGS
        "-T ${CMAKE_CURRENT_SOURCE_DIR}/m4_Debug.ld ${CORE_M4_FLAGS} -Xlinker -Map=dualcore_blinky_m0.map -Xlinker -print-memory-usage -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME}
        lpc_board_ngx_xplorer_4330
        lpc_chip_43xx )

add_custom_command(TARGET ${NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O ihex "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.hex"
        COMMAND ${CMAKE_OBJCOPY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.elf)

add_dependencies(${NAME} m0)

#set(DEVICE LPC4330_M4)
#target_jlink_flash(${NAME} 0x0000000)

