cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(third_library)

# 设置库名称
set(LIBRARY_NAME third_library)

# 收集源文件

# 收集头文件
file(GLOB_RECURSE HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/magic_enum"
)

# 设置 qtkeychain 使用 Qt6
set(BUILD_WITH_QT6 ON CACHE BOOL "Build qtkeychain with Qt6" FORCE)

# 创建静态库（改为静态库避免DLL导出问题）
add_library(${LIBRARY_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)


# 设置库属性
set_target_properties(${LIBRARY_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${HEADERS}"
        # 确保lib文件输出到正确位置
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# 设置包含目录
target_include_directories(${LIBRARY_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(${LIBRARY_NAME} PUBLIC qt6keychain)

# 添加 MSVC 编译器选项
if (MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE
            /utf-8  # 添加 UTF-8 支持
    )
endif ()

# 安装规则
install(TARGETS ${LIBRARY_NAME}
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${LIBRARY_NAME}
)
