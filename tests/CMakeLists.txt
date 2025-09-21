cmake_minimum_required(VERSION 3.11)
project(test_triangles_3d)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_vector test_vector.cc)
target_link_libraries(test_vector GTest::gtest GTest::gtest_main)
