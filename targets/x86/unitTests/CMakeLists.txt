set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.c"
		"*.hpp"
		"*.cpp"
)

add_executable(${NAME} ${SOURCES} )

target_link_libraries(${NAME}
	core
	gtest
	gmock
	gtest_main
	terminal
)

target_format_sources(${NAME} "${SOURCES}")