file(GLOB RUST_HEADERS rust/*.h)

file(GLOB ENTITY_HEADERS entity/*.h)
file(GLOB ENTITY_SOURCES entity/*.cpp)

set(SOURCES
        db_util.h
        db_util.cpp
        ${RUST_HEADERS}
        ${ENTITY_HEADERS}
        ${ENTITY_SOURCES}
)

add_library(dblib STATIC ${SOURCES})
target_include_directories(dblib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/entity
        ${CMAKE_SOURCE_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DATABASE_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/database.lib")
else ()
    set(DATABASE_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/release/database.lib")
endif ()

target_link_libraries(dblib PUBLIC ${DATABASE_LIB_PATH})



