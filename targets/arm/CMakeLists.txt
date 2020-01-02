set(NAME arm)

#ReadVariables(${CMAKE_CURRENT_LIST_DIR}/Makefile "LDSCRIPT")
#string(REPLACE "_FLASH.ld" "" DEVICE ${LDSCRIPT} )
#string(LENGTH ${DEVICE} DEVICE_LENGTH)
#math(EXPR DEVICE_LENGTH "${DEVICE_LENGTH} - 2")
#string(SUBSTRING ${DEVICE} 0 ${DEVICE_LENGTH} DEVICE)

# TODO
set(DEVICE "MKV58F1M0xxx24")
add_definitions(-DCPU_MKV58F1M0VLQ24)
add_definitions(-DCPU_MKV58F1M0VLQ24_cm7)
add_definitions(-DFSL_RTOS_BM)
add_definitions(-DSDK_OS_BAREMETAL)

file(GLOB_RECURSE allDriversFiles
			drivers/*.c
		)

foreach(file ${allDriversFiles})
	set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(file)
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/source/cpp_config.cpp PROPERTIES COMPILE_FLAGS "-w")

string(REPLACE " " ";" PROJECT_DIRECTORIES ${PROJECT_DIRECTORIES} )
foreach(DIRECTORY ${PROJECT_DIRECTORIES})
	file(GLOB_RECURSE DIRECTORY_SOURCES_C ${DIRECTORY}/*.c)
	file(GLOB_RECURSE DIRECTORY_SOURCES_CPP ${DIRECTORY}/*.cpp)

	set(SOURCES_AUTO ${SOURCES_AUTO} ${DIRECTORY_SOURCES_C} ${DIRECTORY_SOURCES_CPP})

	include_directories( ${CMAKE_CURRENT_LIST_DIR}/${DIRECTORY})
endforeach(DIRECTORY)

set(SOURCES ${SOURCES_AUTO} NXP_hal/NXP_gpio.hpp NXP_hal/NXP_uart.hpp NXP_hal/NXP_gpio.cpp NXP_hal/NXP_uart.cpp NXP_hal/NXP_Kitty.hpp NXP_hal/NXP_Kitty.cpp NXP_hal/NXP_display.hpp NXP_hal/NXP_display.cpp HALina_config.hpp NXP_hal/NXP_pwm.hpp NXP_hal/NXP_pwm.cpp NXP_hal/NXP_servo.cpp)

add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME} core )


target_generate_hex(${NAME})

target_memory_report(${NAME})
target_asm_listing(${NAME})

target_format_sources(${NAME} "${SOURCES_PROJECT}")

target_jlink_flash(${NAME} 0x0000000)

target_stlink_flash(${NAME})
