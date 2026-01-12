cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Utils VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库名称
set(LIBRARY_NAME utils)

# 设置源文件
set(SOURCES
        font_helper.cpp
)

# 设置头文件
set(HEADERS
        font_helper.h
        dump_helper.h
        net_helper.h
)

# 查找 Qt 包
find_package(Qt6 COMPONENTS
        Core
        Gui
        REQUIRED
)

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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBDUMP_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/libdump.lib")
    set(SECURE_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/secure_store.lib")
    set(NET_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/netlib.lib")
endif ()


# 链接依赖库
target_link_libraries(${LIBRARY_NAME}
        PUBLIC
        Qt::Core
        Qt::Gui
        PRIVATE
        ${LIBDUMP_PATH}
        ${SECURE_LIB_PATH}
        ${NET_LIB_PATH}
)

# 如果是 MSVC 编译器，设置特定选项
if (MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE
            /W4     # 警告级别
            /utf-8  # 添加 UTF-8 支持
    )
endif ()

# 安装规则
install(TARGETS ${LIBRARY_NAME}
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${LIBRARY_NAME}
)
