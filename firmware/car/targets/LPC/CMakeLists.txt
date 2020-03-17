set(NAME LPC)

set(CORE_M4_DEFINITIONS "-D__USE_LPCOPEN -DCORE_M4")
set(CORE_M0_DEFINITIONS "-D__USE_LPCOPEN -DCORE_M0")

set(CORE_M4_FLAGS "-g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage")
set(CORE_M0_FLAGS "-g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -fstack-usage")

add_subdirectory("lpc_chip_43xx")
add_subdirectory("lpc_chip_43xx_m0")
add_subdirectory("lpc_board_ngx_xplorer_4330_m0")
add_subdirectory("lpc_board_ngx_xplorer_4330")
add_subdirectory("m0")
add_subdirectory("m4")

#set(SOURCES ${SOURCES_AUTO} )
#
#add_executable(${NAME} ${SOURCES})
#
#set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} ${ARMFLOAT} -flto -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -flto -lc")
#
#target_link_libraries(${NAME} core )
#
#
#target_generate_hex(${NAME})
#
#target_memory_report(${NAME})
#target_asm_listing(${NAME})
#
#target_format_sources(${NAME} "${SOURCES_PROJECT}")
#
#target_jlink_flash(${NAME} 0x0000000)
#
#target_stlink_flash(${NAME})
#
#target_generate_elf(${NAME})