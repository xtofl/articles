cmake_minimum_required(VERSION 3.10)

project(GenericTupleHash)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(TestGenericTupleHash tests.cpp)
target_include_directories(TestGenericTupleHash PRIVATE include)
target_link_libraries(TestGenericTupleHash GTest::Main GTest::GTest)
gtest_add_tests(TARGET TestGenericTupleHash SOURCES tests.cpp)