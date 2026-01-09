# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Logger VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库名称
set(LIBRARY_NAME talos_logger)

# 设置源文件
set(SOURCES
    logger.cpp
)

# 设置头文件
set(HEADERS
    logger.h
)

# 创建静态库
add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

# 设置包含目录
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LOGGER_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/logger.lib")
else ()
    set(LOGGER_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/release/logger.lib")
endif ()

target_link_libraries(${LIBRARY_NAME} PUBLIC ${LOGGER_LIB_PATH})

