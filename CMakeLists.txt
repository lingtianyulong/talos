cmake_minimum_required(VERSION 3.19)
project(talos LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "D:/Qt/6.8.3/msvc2022_64")

set(CMAKE_CXX_STANDARD 20)

# 设置不同配置的输出目录，统一放在 build/<Config>
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

foreach(CONFIG_TYPE Debug Release)
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_SOURCE_DIR}/build/${CONFIG_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_SOURCE_DIR}/build/${CONFIG_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_SOURCE_DIR}/build/${CONFIG_TYPE}")
endforeach()

set(RUST_LIBS_ROOT "${CMAKE_SOURCE_DIR}/rust_libs")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets QuickControls2)

# 获取 git 版本
find_package(Git QUIET) 
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_VERSION "unknown")
    message(STATUS "Git not found")
endif()

string(REPLACE "\"" "\\\"" GIT_VERSION_ESCAPED "${GIT_VERSION}")
add_compile_definitions(GIT_VERSION=\"${GIT_VERSION_ESCAPED}\")
message(STATUS "Git commit hash: ${GIT_VERSION}")

configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
    @ONLY
)

include_directories(${CMAKE_BINARY_DIR})

if (WIN32)
    set(APP_RC ${CMAKE_SOURCE_DIR}/version.rc)
endif()



qt_standard_project_setup()

qt_add_executable(talos
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources/resource.qrc
    ${APP_RC}
)

add_subdirectory(logger)         # 然后是 logger
add_subdirectory(config)         # 添加 config 目录
add_subdirectory(utils)          # 添加 utils 目录
add_subdirectory(controls)       # 添加 controls 目录
add_subdirectory(models)         # 添加 models 目录
add_subdirectory(db)             # 添加 db 目录
add_subdirectory(encrypt)
add_subdirectory(capture)
add_subdirectory(ui)             # 最后是 ui


target_link_libraries(talos
    PRIVATE
        utils
        config
        ui
        controls
        capture
        encrypt
        dblib
        talos_models
        talos_logger
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt::QuickWidgets
        Qt::QuickControls2
)

include(GNUInstallDirs)

install(TARGETS talos
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET talos
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

add_custom_command(TARGET talos POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/build/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RUST_LIBS_ROOT}/$<IF:$<CONFIG:Debug>,debug,$<IF:$<CONFIG:Release>,release,release>>"
        "${CMAKE_SOURCE_DIR}/build/$<CONFIG>"
    COMMENT "copy rust_libs to build/$<CONFIG>"
)


