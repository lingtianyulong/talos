cmake_minimum_required(VERSION 3.19)
project(ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)

file(GLOB SOURCES "*.h" "*.cpp" "*.ui") # force re-glob after file removal

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
