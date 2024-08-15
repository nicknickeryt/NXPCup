# @purpose   NXP_CUP 2021 KnightRider::Kitty
# @brief     CMake script defining the build process of the NXP_CUP Project firmware
cmake_minimum_required(VERSION 3.30)

##################################################################################
# Platform definitions
##################################################################################
# always MKV
set(PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/MKV)
if(NOT EXISTS ${PLATFORM_PATH})
  message(FATAL_ERROR "Cos jest nie tak")
endif()
message(STATUS "Target platform MKV")

##################################################################################
# Paths definitions
##################################################################################
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/MKV)
set(REPORTS_PATH ${OUTPUT_PATH}/reports)
file(MAKE_DIRECTORY ${OUTPUT_PATH})

##################################################################################
# Global definitions
##################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/bin)

##################################################################################
# Includes
##################################################################################
# include(functions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/MKV/mkv.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/code_formatting.cmake)
include(${PLATFORM_PATH}/toolchain.cmake)


##################################################################################
# Supports functions
##################################################################################
formatting_support()

##################################################################################
# Build definitions
##################################################################################
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##################################################################################
# Project definition
##################################################################################
project(NXP_CUP C CXX ASM)

##################################################################################
# Additional linker / compiler / subirectories
##################################################################################
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g -DNDEBUG -flto -DLOG_LEVEL=LOG_LEVEL_ALL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG -flto -g -DLOG_LEVEL=LOG_LEVEL_ALL")
add_subdirectory(halina)