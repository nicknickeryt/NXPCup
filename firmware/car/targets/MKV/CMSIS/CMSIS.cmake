add_library(CMSIS STATIC ${CMAKE_CURRENT_LIST_DIR}/system_MKV58F24.c)
target_include_directories(CMSIS PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_FLAGS "-Wno-register")