set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)



set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc" CACHE STRING "Assembler selected" FORCE)

set(CMAKE_C_COMPILER /usr/share/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER /usr/share/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ CACHE PATH "" FORCE)

# set(CMAKE_C_COMPILER "arm-none-eabi-gcc" CACHE STRING "C Compiler selected" FORCE)
# set(CMAKE_CXX_COMPILER "arm-none-eabi-g++" CACHE STRING "C++ Compiler selected" FORCE)
set(CMAKE_LINKER "arm-none-eabi-ld" CACHE STRING "Linker selected" FORCE)
SET(CMAKE_RANLIB "arm-none-eabi-gcc-ranlib" CACHE STRING "Ranlib selected" FORCE)
set(CMAKE_AR "arm-none-eabi-gcc-ar" CACHE STRING "Archiver selected" FORCE)
set(CMAKE_NM "arm-none-eabi-gcc-nm" CACHE STRING "NM selected" FORCE)
set(CMAKE_GCOV "arm-none-eabi-gcov" CACHE STRING "gcov selected" FORCE)
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy" CACHE STRING "objcopy selected" FORCE)
set(CMAKE_OBJDUMP "arm-none-eabi-objdump" CACHE STRING "objdump selected" FORCE)
set(CMAKE_MAKE_PROGRAM "make" CACHE STRING "make selected" FORCE)
set(CMAKE_GCC_SIZE "arm-none-eabi-size" CACHE STRING "size selected" FORCE)
set(CC "arm-none-eabi-gcc")
set(CXX "arm-none-eabi-g++")

set(CMAKE_EXECUTABLE_FORMAT ELF)

set(CLANG_FORMAT "clang-format")

message(STATUS "Using C compiler from ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler from ${CMAKE_CXX_COMPILER}")