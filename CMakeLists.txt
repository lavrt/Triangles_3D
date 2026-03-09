cmake_minimum_required(VERSION 3.11)
project(triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# triangles_3d -------------------------------

add_executable(triangles_3d src/main.cc)

target_include_directories(triangles_3d
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dump
        ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry/acceleration
        ${CMAKE_CURRENT_SOURCE_DIR}/src/details
)

# tests --------------------------------------

enable_testing()
add_subdirectory(tests)
