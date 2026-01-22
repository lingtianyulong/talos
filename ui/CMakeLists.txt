cmake_minimum_required(VERSION 3.19)
project(ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)

file(GLOB SOURCES "*.h" "*.cpp" "*.ui") # force re-glob after file removal

set (ADS_ROOT ${CMAKE_SOURCE_DIR}/third_library/qt-advanced-docking-system)
set_target_properties(qtadvanceddocking PROPERTIES 
    IMPORTED_LOCATION_DEBUG "${ADS_ROOT}/bin/qtadvanceddocking.dll"
    IMPORTED_LOCATION_RELEASE "${ADS_ROOT}/bin/qtadvanceddocking.dll"
    IMPORTED_LOCATION_MINSIZEREL "${ADS_ROOT}/bin/qtadvanceddocking.dll"
    IMPORTED_LOCATION_RELWITHDEBINFO "${ADS_ROOT}/bin/qtadvanceddocking.dll"
    IMPORTED_IMPLIB_DEBUG "${ADS_ROOT}/lib/qtadvanceddockingd.lib"
    IMPORTED_IMPLIB_RELEASE "${ADS_ROOT}/lib/qtadvanceddocking.lib"
    IMPORTED_IMPLIB_MINSIZEREL "${ADS_ROOT}/lib/qtadvanceddocking.lib"
    IMPORTED_IMPLIB_RELWITHDEBINFO "${ADS_ROOT}/lib/qtadvanceddocking.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${ADS_ROOT}/include"
)

add_library(ui STATIC ${SOURCES})

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ui
    PUBLIC
        controls
        talos_logger
        config
        utils
        encrypt
        dblib
        talos_models
        capture
        qtadvanceddocking
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt::QuickWidgets
        Qt::QuickControls2
        
)
