set(NAME command_terminal)

add_library(${NAME} STATIC
    Command.h
        argumets_parser.cpp
    command_manager.h
    cyclicBuffer.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})