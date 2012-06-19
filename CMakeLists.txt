cmake_minimum_required(VERSION 2.6)

project(v8)

set(V8MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(V8INC_DIR ${V8MAIN_DIR}/include)

set(CMAKE_DEBUG_POSTFIX "_d")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(MSVC_BUILD_SYSTEM   1)
    set(RENDER_SYSTEM_D3D11 1)

    if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /Zi /RTC1")
        add_definitions(/DDEBUG /D_DEBUG)
    endif()

    set(V8_COMPILER_SYSTEM  ${MSVC_BUILD_SYSTEM})
    set(V8_RENDER_SYSTEM    ${RENDER_SYSTEM_D3D11})
endif()

if (MINGW)
    set(MINGW_BUILD_SYSTEM  1)
    set(RENDER_SYSTEM_OPENGL 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

    if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        add_definitions(-DDEBUG -D_DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    endif()
    
    set(V8_COMPILER_SYSTEM  ${MINGW_BUILD_SYSTEM})
    set(V8_RENDER_SYSTEM    ${RENDER_SYSTEM_OPENGL})
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/include/v8/config/config.h.in"
    "${PROJECT_BINARY_DIR}/include/v8/config/config.h"
    )

include_directories("${V8INC_DIR}")
include_directories("${PROJECT_BINARY_DIR}/include")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(lib/base)
add_subdirectory(lib/math)

install(
    DIRECTORY include/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.inl"
    )

install(
    DIRECTORY "${PROJECT_BINARY_DIR}/include/"
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.inl"
    )

install(
    DIRECTORY "${PROJECT_BINARY_DIR}/lib/"
    DESTINATION lib
    FILES_MATCHING
    PATTERN "*.a"
    PATTERN "*.lib"
    PATTERN "*.pdb"
    )
