add_library(MKV_hal STATIC  ${CMAKE_CURRENT_LIST_DIR}/MKV_Kitty.cpp 
                            ${CMAKE_CURRENT_LIST_DIR}/MKV_gpio.cpp 
                            )
target_include_directories(MKV_hal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(MKV_hal drivers debug_module halina board) 