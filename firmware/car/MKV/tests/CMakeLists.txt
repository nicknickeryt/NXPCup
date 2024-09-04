cmake_minimum_required(VERSION 3.14)
project(MKV_tests)

# GoogleTest requires at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(requirements.cmake)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  MKV_tests
  algorithm_test.cpp
)

target_link_libraries(
  MKV_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MKV_tests)
