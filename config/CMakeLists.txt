cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Config VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库名称
set(LIBRARY_NAME config)

find_package(Qt6 COMPONENTS Core REQUIRED)

# 设置源文件和头文件
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h"
)

# 创建静态库
add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
)

# 设置库属性，确保输出到正确位置
set_target_properties(${LIBRARY_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    VERSION ${PROJECT_VERSION}
)

# 设置包含目录
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/third_library
        ${CMAKE_SOURCE_DIR}
)

target_compile_definitions(${LIBRARY_NAME} PUBLIC
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:Release>:BUILD_RELEASE>)


# 根据构建类型设置 talos_sys_config 库路径
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TALOS_SYS_CONFIG_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/talos_sys_config.dll.lib")
else()
    set(TALOS_SYS_CONFIG_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/release/talos_sys_config.dll.lib")
endif()

# 检查库文件是否存在
if(EXISTS ${TALOS_SYS_CONFIG_LIB_PATH})
    # 链接依赖库
    target_link_libraries(${LIBRARY_NAME}
        PUBLIC
            third_library
            ${TALOS_SYS_CONFIG_LIB_PATH}
            Qt::Core
    )
else()
#    message(WARNING "talos_sys_config library not found at: ${TALOS_SYS_CONFIG_LIB_PATH}")
    # 链接依赖库（不包含 talos_sys_config）
    target_link_libraries(${LIBRARY_NAME}
        PUBLIC
            third_library Qt::Core
    )
endif()

# 如果是 MSVC 编译器，设置特定选项
if(MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE
        /W4     # 警告级别
        /WX     # 将警告视为错误
        /utf-8  # 添加 UTF-8 支持
    )
endif()

# 安装规则
install(TARGETS ${LIBRARY_NAME}
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${LIBRARY_NAME}
)
