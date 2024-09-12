cmake_minimum_required(VERSION 3.14.3)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

add_executable(PropertyTest PropertyTest.cpp)
target_link_libraries(PropertyTest reflection)

add_executable(PropertyTestOOP PropertyTestOOP.cpp)
target_link_libraries(PropertyTestOOP reflection)