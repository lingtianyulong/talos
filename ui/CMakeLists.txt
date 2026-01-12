cmake_minimum_required(VERSION 3.19)
project(ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick QuickWidgets)

file(GLOB SOURCES  *.h *.cpp *.ui)

add_library(ui STATIC ${SOURCES})

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
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt::QuickWidgets
)
