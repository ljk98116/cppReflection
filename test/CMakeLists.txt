cmake_minimum_required(VERSION 3.14.3)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

add_executable(PropertyTest PropertyTest.cpp)
target_link_libraries(PropertyTest reflection)

add_executable(PropertyTestOOP PropertyTestOOP.cpp)
target_link_libraries(PropertyTestOOP reflection)

add_executable(MethodTest1 MethodTest1.cpp)
target_link_libraries(MethodTest1 reflection)

add_executable(EnumTest EnumTest.cpp)
target_link_libraries(EnumTest reflection)