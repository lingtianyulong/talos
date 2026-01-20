cmake_minimum_required(VERSION 3.19)
project(algo_plugin LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

file (GLOB SOURCES *.cpp *.h)
file (GLOB_RECURSE RUST_SOURCES rust_core/*.h)

add_library(algo_plugin SHARED ${SOURCES} ${RUST_SOURCES})

target_include_directories(algo_plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB DEBUG_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/algo_libs/debug/*.lib")
    target_link_libraries(algo_plugin PRIVATE ${DEBUG_LIBS})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(GLOB RELEASE_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/algo_libs/release/*.lib")
    target_link_libraries(algo_plugin PRIVATE ${RELEASE_LIBS})
endif()

target_link_libraries(algo_plugin PRIVATE talos_logger Qt6::Core)
