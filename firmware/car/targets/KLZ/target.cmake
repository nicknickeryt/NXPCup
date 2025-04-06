set(NAME KLZ)

set(DEVICE "CPU_MKL25Z128xxx4")
add_compile_definitions(	
							CPU_MKL25Z128VLK4
							CPU_MKL25Z128VLK4_cm0plus
							CPU_MKL25Z128xxx4
							FSL_RTOS_BM
							SDK_OS_BAREMETAL
							__USE_CMSIS
						)

include(${CMAKE_CURRENT_LIST_DIR}/board/board.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMSIS/CMSIS.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/debug_module/debug_module.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/drivers/drivers.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/NXP_hal/NXP_hal.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake)

set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/source/cpp_config.cpp
			# ${CMAKE_CURRENT_LIST_DIR}/source/commandManager.cpp
			${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
			${CMAKE_CURRENT_LIST_DIR}/source/semihost_hardfault.c)

add_executable(${NAME} ${SOURCES})
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/source)

set (CWARN "-Wall -Wstrict-prototypes -Wextra ")
set (CXXWARN "-Wall -Wextra")
set (CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
set (ARMFLOAT "-mfloat-abi=soft")
set (CMCU "-mcpu=cortex-m0plus -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -std=gnu++2a -fconcepts -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/source/cpp_config.cpp PROPERTIES COMPILE_FLAGS "-w")

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/MKL25Z128xxx4.ld)
set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} ${ARMFLOAT} -flto -Wl,--gc-sections  -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -specs=nano.specs -specs=nosys.specs -flto -lc")

target_link_libraries(${NAME} halina board CMSIS debug_module drivers NXP_hal)


add_custom_command(TARGET ${NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O ihex "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.hex"
        COMMAND ${CMAKE_OBJCOPY} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME}.elf")

target_stlink_flash(${NAME})
# arm-none-eabi-gcc -nostdlib -Xlinker --gc-sections -Xlinker -Map="frdmkl25z_driver_examples_i2c_dma_transfer.map" -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T frdmkl25z_driver_examples_i2c_dma_transfer_Debug.ld -o "frdmkl25z_driver_examples_i2c_dma_transfer.axf" ./utilities/fsl_debug_console.o  ./startup/startup_mkl25z4.o  ./source/i2c_dma_transfer.o ./source/mtb.o ./source/semihost_hardfault.o  ./drivers/fsl_clock.o ./drivers/fsl_common.o ./drivers/fsl_dma.o ./drivers/fsl_dmamux.o ./drivers/fsl_flash.o ./drivers/fsl_gpio.o ./drivers/fsl_i2c.o ./drivers/fsl_i2c_dma.o ./drivers/fsl_lpsci.o ./drivers/fsl_lpsci_dma.o ./drivers/fsl_smc.o ./drivers/fsl_uart.o ./drivers/fsl_uart_dma.o  ./board/board.o ./board/clock_config.o ./board/pin_mux.o  ./CMSIS/system_MKL25Z4.o   
