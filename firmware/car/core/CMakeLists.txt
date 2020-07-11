set(NAME core)

file(GLOB_RECURSE SOURCES
	"*.h"
	"*.c"
	"*.cpp"
	"*.hpp"
)

message("SOURCES: ${SOURCES}")

add_library(${NAME}
	${SOURCES})

target_link_libraries(${NAME}
	msftgsl
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(external)
add_subdirectory(command_terminal)
