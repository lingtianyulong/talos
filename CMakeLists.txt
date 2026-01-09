cmake_minimum_required(VERSION 3.30)
project(talos VERSION 1.0)

# 基础设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 添加 MSVC UTF-8 支持
if (MSVC)
    add_compile_options(/utf-8)
endif ()

# 禁用GLOB_RECURSE的自动更新
cmake_policy(SET CMP0009 NEW)

# 设置库输出目录，确保所有库文件都在同一目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Qt 路径设置
if (WIN32)
    set(CMAKE_PREFIX_PATH "D:/Qt/6.8.3/msvc2022_64")
endif ()

# 查找 Qt 包
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED
)

# 添加子目录 - 调整构建顺序
add_subdirectory(third_library)  # 首先构建 third_library
add_subdirectory(logger)         # 然后是 logger
add_subdirectory(config)         # 添加 config 目录
add_subdirectory(utils)          # 添加 utils 目录
add_subdirectory(controls)       # 添加 controls 目录
add_subdirectory(models)         # 添加 models 目录
add_subdirectory(db)             # 添加 db 目录
add_subdirectory(encrypt)
add_subdirectory(capture)
add_subdirectory(ui)             # 最后是 ui

# 添加资源文件
qt_add_resources(FONT_RESOURCES ${CMAKE_SOURCE_DIR}/resources/resources.qrc)

# 主程序
add_executable(${PROJECT_NAME}
        main.cpp
        ${FONT_RESOURCES}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:Release>:BUILD_RELEASE>)

# 链接库 - 注意顺序很重要
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        talos_logger          # 链接 logger(底层逻辑, 使用 rust 开发)
        config                # 添加 config 库
        utils                 # 添加 utils 库
        controls              # 添加 controls 库
        talos_models          # 添加 models 库
        dblib                 # 添加 dblib
        encrypt
        capture
        talos_ui              # 添加 ui 库
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Windows 平台特定设置
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    set_target_properties(talos PROPERTIES
            WIN32_EXECUTABLE TRUE  # 设置为 GUI 程序
            LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"  # 可选：明确指定入口"
    )

    # 查找 Qt 安装路径
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # 复制 Qt 平台插件
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    # 复制 Qt 依赖库
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
