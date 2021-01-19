add_library(NXP_hal STATIC  ${CMAKE_CURRENT_LIST_DIR}/NXP_gpio.cpp 
                            ${CMAKE_CURRENT_LIST_DIR}/NXP_I2C.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/NXP_Kitty.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/NXP_uart.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/VL53L0X.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/TCA95x5.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/NXP_algorithm.cpp 
                            ${CMAKE_CURRENT_LIST_DIR}/logger.cpp 
                            ${CMAKE_CURRENT_LIST_DIR}/NXP_sensors.cpp 
                            )
target_include_directories(NXP_hal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(NXP_hal drivers debug_module halina board) 