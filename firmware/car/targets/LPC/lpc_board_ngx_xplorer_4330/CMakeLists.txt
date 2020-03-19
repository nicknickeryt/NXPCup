set(NAME lpc_board_ngx_xplorer_4330)

add_definitions(${CORE_M4_DEFINITIONS} -DCHIP_LPC43XX -DLPC_CREG)
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

add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/config_43xx)

#target_format_sources(${NAME} "${SOURCES}")