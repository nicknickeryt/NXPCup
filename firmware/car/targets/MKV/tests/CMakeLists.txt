cmake_minimum_required(VERSION 3.14)
project(MKV_tests)

set(MKV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(ALGORITHMS_DIR ${MKV_DIR}/algorithms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(requirements.cmake)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  MKV_tests
  algorithm_test.cpp
  pid_test.cpp
  ${ALGORITHMS_DIR}/pid.cpp
  ${ALGORITHMS_DIR}/algorithm.cpp
)

target_include_directories(
  MKV_tests
  PRIVATE
  ${ALGORITHMS_DIR})

target_link_libraries(
  MKV_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MKV_tests)
