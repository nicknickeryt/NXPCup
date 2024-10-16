add_library(algorithms STATIC   ${CMAKE_CURRENT_LIST_DIR}/algorithm.cpp 
                                ${CMAKE_CURRENT_LIST_DIR}/pid.cpp
                                )
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(algorithms drivers halina NXP_hal)