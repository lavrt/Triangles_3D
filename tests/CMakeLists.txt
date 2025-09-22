find_package(GTest REQUIRED)

set(SOURCES
    test_main.cc
    test_vector.cc ../src/vector.cc
    test_point.cc ../src/point.cc
)

add_executable(run_tests ${SOURCES})

target_include_directories(run_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(run_tests)
