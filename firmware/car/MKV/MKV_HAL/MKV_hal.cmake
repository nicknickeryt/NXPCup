add_library(MKV_hal STATIC  ${CMAKE_CURRENT_LIST_DIR}/MKV_Kitty.cpp 
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_gpio.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_pwm.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_servo.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_display.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_motor.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_uart.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_DMA.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_adc.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_PIT.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_camera.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_menu.cpp
                            )
target_include_directories(MKV_hal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(MKV_hal drivers debug_module halina board) 