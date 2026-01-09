cmake_minimum_required(VERSION 3.30)

project(capture VERSION 1.0)

set(LIB_NAME capture)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/rust/*.h"
)

add_library(${LIB_NAME} STATIC ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/screen_capture.lib")
else ()
    set(LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/release/screen_capture.lib")
endif ()

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIB_NAME} PRIVATE ${LIB_PATH})