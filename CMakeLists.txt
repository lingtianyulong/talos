cmake_minimum_required(VERSION 3.19)
project(talos LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "D:/Qt/6.8.3/msvc2022_64")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_QPA_PLATFORM_PLUGIN_PATH "D:/Qt/6.8.3/msvc2022_64/plugins/platforms")


find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(talos
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

add_subdirectory(ui)


target_link_libraries(talos
    PRIVATE
        ui
        Qt::Core
        Qt::Widgets
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
