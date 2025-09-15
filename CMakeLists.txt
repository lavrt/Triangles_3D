cmake_minimum_required(VERSION 3.11)
project(triangles)

set(SOURCES
    src/main.cc
)

add_executable(triangles_3d ${SOURCES})

target_include_directories(triangles_3d
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
