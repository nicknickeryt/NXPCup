file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/*.c
)

foreach(file ${SOURCES})
    set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(file)

add_library(drivers STATIC ${SOURCES})
target_include_directories(drivers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(drivers CMSIS)