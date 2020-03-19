set(NAME lpc_chip_43xx_m0)

add_definitions(${CORE_M0_DEFINITIONS} -DCHIP_LPC43XX -DLPC_CREG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CORE_M0_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORE_M0_FLAGS}")

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
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/config_43xx_m0app)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_rom)