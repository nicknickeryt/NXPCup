set(NAME core)

file(GLOB_RECURSE SOURCES
	"*.h"
	"*.c"
	"*.cpp"
	"*.hpp"
)

message("SOURCES: ${SOURCES}")

add_library(${NAME}
	${SOURCES} core.cpp HALina_gpio.hpp HALina_pwm.hpp HALina_motor.hpp)

target_link_libraries(${NAME}
	msftgsl
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_format_sources(${NAME} "${SOURCES}")

add_subdirectory(external)
