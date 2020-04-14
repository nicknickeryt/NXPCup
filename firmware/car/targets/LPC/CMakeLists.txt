set(NAME LPC)

set(CORE_M4_DEFINITIONS "-D__USE_LPCOPEN -DCORE_M4")
set(CORE_M0_DEFINITIONS "-D__USE_LPCOPEN -DCORE_M0")

set(CORE_M4_FLAGS "-g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage")
set(CORE_M0_FLAGS "-g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -fstack-usage")

add_subdirectory("m0")
add_subdirectory("m4")
