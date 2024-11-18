cmake_minimum_required(VERSION 3.22.0)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

enable_testing()
add_compile_options(-g)
add_executable(PropertyTest PropertyTest.cc)
target_link_libraries(PropertyTest reflection GTest::gtest_main)

add_executable(MethodTest MethodTest.cc)
target_link_libraries(MethodTest reflection GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(PropertyTest)
gtest_discover_tests(MethodTest)
