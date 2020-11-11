add_library(algorithms STATIC   ${CMAKE_CURRENT_LIST_DIR}/algorithm_unit.cpp 
                                ${CMAKE_CURRENT_LIST_DIR}/obstacle_detector.cpp
                                ${CMAKE_CURRENT_LIST_DIR}/patterns_detector.cpp
                                ${CMAKE_CURRENT_LIST_DIR}/track_lines_detector.cpp
                                )
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(algorithms drivers halina NXP_hal)