cmake_minimum_required(VERSION 3.5)

project(mpga LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES mpga_en_GB.ts)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    inc/*.h
    src/*.ui
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations ")

add_executable(mpga
    ${SOURCES}
    ${TS_FILES}
)

target_link_libraries(mpga PRIVATE Qt5::Widgets)
target_link_libraries(mpga PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libusb-1.0.dll.a")
target_include_directories(mpga PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc/" )

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
