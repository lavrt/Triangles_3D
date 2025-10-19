# GTest -----------------------------------------------------------

find_package(GTest REQUIRED)

set(SOURCES_GTEST
    gtest/test_triangle.cc
    gtest/test_bvh.cc
    gtest/test_triangle.cc
    gtest/test_segment.cc
    gtest/test_vector.cc
    gtest/test_point.cc
    gtest/test_aabb.cc
    gtest/test_node.cc
    gtest/test_main.cc
)

add_executable(run_gtest ${SOURCES_GTEST})

target_include_directories(run_gtest
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(run_gtest GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_gtest)

# E2E tests -------------------------------------------------------

## test generation ------------------

add_executable(run_e2e_generation e2e/test_generation/main.cc)

## test execution -------------------

add_executable(run_e2e_test_execution e2e/test_execution/main.cc)

target_include_directories(run_e2e_test_execution
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

## reference program ----------------

find_package(fcl REQUIRED)

add_executable(run_e2e_reference e2e/reference/main.cc)

target_link_libraries(run_e2e_reference
    PUBLIC
        fcl
)


