add_library(board STATIC    ${CMAKE_CURRENT_LIST_DIR}/board.c 
                            ${CMAKE_CURRENT_LIST_DIR}/clock_config.c
                            ${CMAKE_CURRENT_LIST_DIR}/peripherals.c
                            ${CMAKE_CURRENT_LIST_DIR}/pin_mux.c
                            ${CMAKE_CURRENT_LIST_DIR}/startup_mkv58f24.cpp
                            )
target_include_directories(board PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(board drivers)