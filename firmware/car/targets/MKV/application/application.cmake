add_library(application STATIC ${CMAKE_CURRENT_LIST_DIR}/uart_frame.cpp
)

target_include_directories(application PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)