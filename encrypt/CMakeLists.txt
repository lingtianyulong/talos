set(CMAKE_CXX_STANDARD 20)

file(GLOB RUST_HEADERS rust/*.h)

set(ENCRYPT_SOURCES
        ${RUST_HEADERS}
        encrypt_util.cpp
        encrypt_util.h
)

set(LIBRARY_NAME encrypt)

add_library(${LIBRARY_NAME} STATIC ${ENCRYPT_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${RUST_HEADERS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ENCRYPT_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/debug/encryption.lib")
else ()
    set(ENCRYPT_LIB_PATH "${CMAKE_SOURCE_DIR}/rust_libs/release/encryption.lib")
endif ()

target_link_libraries(${LIBRARY_NAME} PUBLIC ${ENCRYPT_LIB_PATH} Microsoft.GSL::GSL)