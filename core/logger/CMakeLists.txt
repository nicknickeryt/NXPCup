set(NAME logger)

add_library(${NAME} STATIC
        logger.h
        logger.cpp
        )

target_include_directories(${NAME} PUBLIC .)
