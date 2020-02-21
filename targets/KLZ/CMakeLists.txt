set(NAME KLZ)

# TODO
set(DEVICE "CPU_MKL25Z128xxx4")
add_definitions(-DCPU_MKL25Z128VLK4)
add_definitions(-DCPU_MKL25Z128VLK4_cm0plus)
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

set(SOURCES ${SOURCES_AUTO} )

add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} ${ARMFLOAT} -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME} core )


target_generate_hex(${NAME})

target_memory_report(${NAME})
target_asm_listing(${NAME})

target_format_sources(${NAME} "${SOURCES_PROJECT}")

target_jlink_flash(${NAME} 0x0000000)

target_stlink_flash(${NAME})

target_generate_elf(${NAME})