add_library(algorithms STATIC   ${CMAKE_CURRENT_LIST_DIR}/A_camera.cpp 
                                ${CMAKE_CURRENT_LIST_DIR}/A_motor.cpp
                                ${CMAKE_CURRENT_LIST_DIR}/A_servo.cpp
                                )
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(algorithms drivers halina NXP_hal)