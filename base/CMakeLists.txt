cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME talos_base_lib)
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME})

file(GLOB RUST_FILES "rust/*.h")
file(GLOB_RECURSE SRC_FILES "*.cpp" "*.h")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE BASE_DLL_EXPORT)

# 使用生成器表达式来处理 Debug/Release 路径
set(RUST_LIB_DIR "${CMAKE_SOURCE_DIR}/rust_libs")
set(LIB_BASE_PATH "$<IF:$<CONFIG:Debug>,${RUST_LIB_DIR}/debug/lib_base.lib,${RUST_LIB_DIR}/release/lib_base.lib>")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_BASE_PATH} Microsoft.GSL::GSL)