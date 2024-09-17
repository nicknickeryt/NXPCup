set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARM_COMPILER_PATH /usr/share/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin)

set(CMAKE_C_COMPILER ${ARM_COMPILER_PATH}/arm-none-eabi-gcc CACHE STRING "C Compiler selected" FORCE)
set(CMAKE_CXX_COMPILER ${ARM_COMPILER_PATH}/arm-none-eabi-g++ CACHE STRING "C++ Compiler selected" FORCE)
set(CMAKE_LINKER ${ARM_COMPILER_PATH}/arm-none-eabi-ld CACHE STRING "Linker selected" FORCE)
SET(CMAKE_RANLIB ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-ranlib CACHE STRING "Ranlib selected" FORCE)
set(CMAKE_AR ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-ar CACHE STRING "Archiver selected" FORCE)
set(CMAKE_NM ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-nm CACHE STRING "NM selected" FORCE)
set(CMAKE_GCOV ${ARM_COMPILER_PATH}/arm-none-eabi-gcov CACHE STRING "gcov selected" FORCE)
set(CMAKE_OBJCOPY ${ARM_COMPILER_PATH}/arm-none-eabi-objcopy CACHE STRING "objcopy selected" FORCE)
set(CMAKE_OBJDUMP ${ARM_COMPILER_PATH}/arm-none-eabi-objdump CACHE STRING "objdump selected" FORCE)
set(CMAKE_MAKE_PROGRAM "ninja" CACHE STRING "make selected" FORCE)
set(CMAKE_GCC_SIZE ${ARM_COMPILER_PATH}/arm-none-eabi-size CACHE STRING "size selected" FORCE)
set(CC ${ARM_COMPILER_PATH}/arm-none-eabi-gcc)
set(CXX ${ARM_COMPILER_PATH}/arm-none-eabi-g++)

set(CMAKE_EXECUTABLE_FORMAT ELF)

set(CLANG_FORMAT "clang-format")

message(STATUS "Using C compiler from ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler from ${CMAKE_CXX_COMPILER}")