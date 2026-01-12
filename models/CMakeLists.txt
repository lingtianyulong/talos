cmake_minimum_required(VERSION 3.30)

# 设置项目名称和版本
project(talos_models VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

# 设置库名称
set(LIBRARY_NAME talos_models)

# 设置源文件
file(GLOB_RECURSE SOURCES "*.cpp")

# 设置头文件
file(GLOB_RECURSE HEADERS "*.h")

# 创建静态库
add_library(${LIBRARY_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

# 设置库属性，确保输出到正确位置
set_target_properties(${LIBRARY_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${HEADERS}"
)

# 设置包含目录
target_include_directories(${LIBRARY_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${LIBRARY_NAME} PUBLIC Qt6::Core)