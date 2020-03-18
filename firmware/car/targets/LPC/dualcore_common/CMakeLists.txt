set(NAME dualcore_common)

#add_definitions(${CORE_M0_DEFINITIONS})
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CORE_M0_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORE_M0_FLAGS}")

file(GLOB_RECURSE SOURCES
        "*.h"
        "*.c"
        "*.cpp"
        "*.hpp"
        )

message("SOURCES ${NAME}: ${SOURCES}")

add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx/inc/config_43xx)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lpc_chip_43xx_m0/inc/config_43xx_m0app)