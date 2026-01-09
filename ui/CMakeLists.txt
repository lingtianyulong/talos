cmake_minimum_required(VERSION 3.30)

# Find the Qt package
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Tell CMake to run moc when necessary
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run uic automatically when needed
set(CMAKE_AUTOUIC ON)

file(GLOB WIDGET_HEADERS widgets/*.h)
file(GLOB WIDGET_SOURCES widgets/*.cpp)
file(GLOB WIDGET_UIFILES widgets/*.ui)

file(GLOB 3D_HEADERS 3d/*.h)
file(GLOB 3D_SOURCES 3d/*.cpp)
file(GLOB 3D_UIFILES 3d/*.ui)


# Source files
set(
        UI_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        ${WIDGET_HEADERS}
        ${WIDGET_SOURCES}
        ${WIDGET_UIFILES}

        ${3D_HEADERS}
        ${3D_SOURCES}
        ${3D_UIFILES}
)


# Create the library
add_library(talos_ui STATIC
        ${UI_SOURCES}
)

target_compile_definitions(talos_ui PRIVATE
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:Release>:BUILD_RELEASE>)

# Link against Qt libraries
target_link_libraries(talos_ui PUBLIC
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        OpenGL::GL
)

# Include directories
target_include_directories(talos_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/widgets
        ${CMAKE_SOURCE_DIR}/third_library
        ${CMAKE_SOURCE_DIR}
)