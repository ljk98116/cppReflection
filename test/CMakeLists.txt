cmake_minimum_required(VERSION 3.14.3)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

enable_testing()
add_compile_options(-g)
add_executable(PropertyTest PropertyTest.cc)
target_link_libraries(PropertyTest reflection GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(PropertyTest)
