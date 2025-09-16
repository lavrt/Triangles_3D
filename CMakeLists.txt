cmake_minimum_required(VERSION 3.11)
project(triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cc
    src/aabb.cc
)

add_executable(triangles_3d ${SOURCES})

target_include_directories(triangles_3d
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
