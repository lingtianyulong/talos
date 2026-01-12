cmake_minimum_required(VERSION 3.30)

project(controls VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME controls)

# Enable Qt MOC (Meta Object Compiler) for Q_OBJECT classes
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h")

add_library(controls STATIC ${SOURCES})

# Find the required Qt packages
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

#add_subdirectory(buttons)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with Qt libraries
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
