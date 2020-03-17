set(NAME m4)

add_definitions(${CORE_M4_DEFINITIONS} -D__LPC43XX__ -DLPC43_MULTICORE_M0APP -D__MULTICORE_MASTER -D__MULTICORE_MASTER_SLAVE_M0APP)
set(CORE_M4_FLAGS "-fno-common -g3 -Wall -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CORE_M4_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORE_M4_FLAGS}")

file(GLOB_RECURSE SOURCES
        "*.h"
        "*.c"
        "*.cpp"
        "*.hpp"
        )

message("SOURCES: ${SOURCES}")

# without warnings
foreach(file ${SOURCES})
    set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(file)

SET(OBJS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/m0.o ../dualcore_common/IPC.h)

SET_SOURCE_FILES_PROPERTIES(
        ${OBJS}
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)

add_executable(${NAME} ${SOURCES}  ${OBJS} )
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dualcore_common/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_board_ngx_xplorer_4330/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/config_43xx/)

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/m4_Debug.ld ${CORE_M4_FLAGS} -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME}
        lpc_board_ngx_xplorer_4330
        lpc_chip_43xx )

add_custom_command(TARGET ${NAME}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy  -v -O ihex "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.hex")

add_dependencies(${NAME} m0)

