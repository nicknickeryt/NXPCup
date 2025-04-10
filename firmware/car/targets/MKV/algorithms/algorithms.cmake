add_library(algorithms STATIC   ${CMAKE_CURRENT_LIST_DIR}/camera.cpp 
                                ${CMAKE_CURRENT_LIST_DIR}/motor.cpp
                                ${CMAKE_CURRENT_LIST_DIR}/pid.cpp
                                )
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)
target_link_libraries(algorithms drivers halina NXP_hal)