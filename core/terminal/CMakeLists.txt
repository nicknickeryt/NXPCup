set(NAME terminal)

add_library(${NAME} STATIC
    terminal.cpp
    terminal.h
)

target_link_libraries(${NAME}
    msftgsl
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})