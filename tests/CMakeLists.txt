find_package(GTest REQUIRED)

set(SOURCES
    test_triangle.cc ../src/triangle.cc
    test_bvh.cc ../src/bvh.cc
    test_node.cc
    test_main.cc
)

add_executable(run_tests ${SOURCES})

target_include_directories(run_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(run_tests)
