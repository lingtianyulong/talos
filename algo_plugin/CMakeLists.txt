cmake_minimum_required(VERSION 3.19)
project(algo_plugin LANGUAGES CXX)

file (GLOB SOURCES *.cpp *.h)
file (GLOB_RECURSE RUST_SOURCES rust_core/*.h)

add_library(algo_plugin SHARED ${SOURCES} ${RUST_SOURCES})

target_include_directories(algo_plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
