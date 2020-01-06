function(target_format_sources TARGET SOURCES)
    if(CLANG_FORMAT)
        add_custom_target(${NAME}.format
                COMMAND ${CLANG_FORMAT} -style=file -i ${SOURCES}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Formatting files: '${SOURCES}'"
                )
    else(CLANG_FORMAT)
        add_custom_target(${NAME}.format
                COMMAND ""
                COMMENT "Clang format has not been found. Code formatting is not available. "
                )
    endif()
endfunction(target_format_sources)

function(target_generate_hex TARGET)
    set (EXEC_OBJ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET})
    set (HEX_OBJ ${EXEC_OBJ}.hex)

    set_target_properties(${TARGET} PROPERTIES HEX_FILE ${HEX_OBJ})

    add_custom_command(OUTPUT ${HEX_OBJ}
            COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXEC_OBJ} ${HEX_OBJ}
            DEPENDS ${TARGET}.size
            )

    add_custom_target (${TARGET}.hex ALL DEPENDS ${HEX_OBJ})
endfunction(target_generate_hex)

function(target_memory_report TARGET)
    get_property(binary TARGET ${TARGET} PROPERTY RUNTIME_OUTPUT_NAME)

    add_custom_command(OUTPUT ${TARGET}.size
            COMMAND ${CMAKE_GCC_SIZE} -A -d $<TARGET_FILE:${TARGET}>
            COMMAND ${CMAKE_GCC_SIZE} -A -d $<TARGET_FILE:${TARGET}> > ${TARGET}.size
            COMMAND ${CMAKE_GCC_SIZE} -B -d $<TARGET_FILE:${TARGET}>
            DEPENDS ${TARGET}
            )
    set_property(TARGET ${NAME} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.map")
endfunction(target_memory_report)

function(target_asm_listing TARGET)
    get_property(binary TARGET ${TARGET} PROPERTY RUNTIME_OUTPUT_NAME)

    add_custom_target(${TARGET}.asm
            COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORTS_PATH}
            COMMAND ${CMAKE_OBJDUMP} -dSC $<TARGET_FILE:${TARGET}> > ${REPORTS_PATH}/${TARGET}.lss
            DEPENDS ${TARGET}
            )
endfunction(target_asm_listing)

function(target_generate_elf TARGET)
    set (EXEC_OBJ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET})
    set (ELF_OBJ ${EXEC_OBJ}.elf)

    set_target_properties(${TARGET} PROPERTIES ELF_FILE ${ELF_OBJ})

    add_custom_command(OUTPUT ${ELF_OBJ}
            COMMAND ${CMAKE_OBJCOPY} ${EXEC_OBJ} ${ELF_OBJ}
            DEPENDS ${TARGET}.size
            )

    add_custom_target (${TARGET}.elf ALL DEPENDS ${ELF_OBJ})
endfunction(target_generate_elf)

#function(ReadVariables MKFile VARIABLE_NAME)
#    if (EXISTS "${MKFile}")
#        file(READ "${MKFile}" FileContents)
#        string(REPLACE "\\\n" "" FileContents ${FileContents})
#        string(REPLACE "\n" ";" FileLines ${FileContents})
#        list(REMOVE_ITEM FileLines "")
#        foreach(line ${FileLines})
#            string(REPLACE "=" ";" line_split ${line})
#            list(LENGTH line_split count)
#            if (count LESS 2)
#                continue()
#            endif()
#            list(GET line_split -1 value)
#            string(STRIP ${value} value)
#            separate_arguments(value)
#            list(REMOVE_AT line_split -1)
#            foreach(var_name ${line_split})
#                string(STRIP ${var_name} var_name)
#                if (var_name STREQUAL VARIABLE_NAME)
#                    set(${var_name} ${value} PARENT_SCOPE)
#                endif()
#
#            endforeach()
#        endforeach()
#    endif()
#endfunction()

function(target_jlink_flash TARGET BASE_ADDRESS)
    set(COMMAND_FILE ${CMAKE_BINARY_DIR}/jlink/${TARGET}.flash.jlink)

    get_property(FLASH_FILE TARGET ${TARGET} PROPERTY HEX_FILE)

    if("${FLASH_FILE}" STREQUAL "")
        get_property(FLASH_FILE TARGET ${TARGET} PROPERTY BIN_FILE)
    endif()

    configure_file(${CMAKE_SOURCE_DIR}/jlink/flash.jlink.template ${COMMAND_FILE})

    unset(FLASH_FILE)

    message("Device ${DEVICE}")

    set(JLINK_ARGS
            "-device" ${DEVICE}
            "-ExitOnError"
            "-CommanderScript" ${COMMAND_FILE}
            )

    if(NOT ${JLINK_SN} STREQUAL "")
        list(APPEND JLINK_ARGS -SelectEmuBySN ${JLINK_SN})
    endif()

    add_custom_target(${TARGET}.flash_jlink
            COMMAND ${JLINK} ${JLINK_ARGS}
            DEPENDS ${TARGET}.hex ${FLASH_FILE}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            )

endfunction(target_jlink_flash)

function(target_stlink_flash TARGET)
    add_custom_target(${TARGET}.flash_stlink
            openocd -f interface/stlink-v2-1.cfg
            -c "transport select hla_swd"
            -f target/stm32f1x.cfg
            -c "init"
            -c "reset halt"
            -c "flash write_image erase ${TARGET}"
            -c "reset run"
            -c "shutdown"
            DEPENDS ${TARGET}
            COMMENT "Flashing target hardware"
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction(target_stlink_flash)

function(target_generate_elf TARGET)
    set (EXEC_OBJ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET})
    set (ELF_OBJ ${EXEC_OBJ}.elf)

    set_target_properties(${TARGET} PROPERTIES ELF_FILE ${ELF_OBJ})

    add_custom_command(OUTPUT ${ELF_OBJ}
            COMMAND ${CMAKE_OBJCOPY} ${EXEC_OBJ} ${ELF_OBJ}
            DEPENDS ${TARGET}.size
            )

    add_custom_target (${TARGET}.elf ALL DEPENDS ${ELF_OBJ})
endfunction(target_generate_elf)

#function(load_submodules)
#    find_package(Git QUIET)
#    set(SEARCH_STATUS "SEARCH SUBMODULE")
#    if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
#        # Update submodules as needed
#        option(GIT_SUBMODULE "Check submodules during build" ON)
#        if(GIT_SUBMODULE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules")
#            file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules" FileContents)
#            string(REPLACE "\\\n" "" FileContents ${FileContents})
#            string(REPLACE "\n" ";" FileLines ${FileContents})
#            list(REMOVE_ITEM FileLines "")
#
#            foreach(line ${FileLines})
#                if (${SEARCH_STATUS} STREQUAL "READ URL")
#                    set(MODULE_URL ${line})
#                    set(SEARCH_STATUS "SEARCH SUBMODULE")
#
#                    string(REPLACE "path = " "" MODULE_PATH ${MODULE_PATH})
#                    string(REPLACE "url = " "" MODULE_URL ${MODULE_URL})
#
#                    execute_process(COMMAND ${GIT_EXECUTABLE} submodule add ${MODULE_URL} ${MODULE_PATH})
#                    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update)
#                endif()
#
#                if (${SEARCH_STATUS} STREQUAL "READ PATH")
#                    set(MODULE_PATH ${line})
#                    set(SEARCH_STATUS "READ URL")
#                endif()
#
#                if (${SEARCH_STATUS} STREQUAL "SEARCH SUBMODULE")
#                    string(FIND ${line} "submodule" FOUND_VARIABLE)
#                    if (${FOUND_VARIABLE} EQUAL 1)
#                        set(SEARCH_STATUS "READ PATH")
#                    endif()
#                endif()
#            endforeach()
#        endif()
#    endif()
#endfunction(load_submodules)
