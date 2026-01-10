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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_QPA_PLATFORM_PLUGIN_PATH "D:/Qt/6.8.3/msvc2022_64/plugins/platforms")


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets QuickControls2)

qt_standard_project_setup()

qt_add_executable(talos
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources/resource.qrc
)

add_subdirectory(utils)
add_subdirectory(ui)


target_link_libraries(talos
    PRIVATE
        utils
        ui
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
