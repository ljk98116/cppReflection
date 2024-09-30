cmake_minimum_required(VERSION 3.14.3)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

add_executable(PropertyExample PropertyExample.cpp)
target_link_libraries(PropertyExample reflection)

add_executable(PropertyOOPExample PropertyOOPExample.cpp)
target_link_libraries(PropertyOOPExample reflection)

add_executable(MethodExample MethodExample.cpp)
target_link_libraries(MethodExample reflection)

add_executable(EnumExample EnumExample.cpp)
target_link_libraries(EnumExample reflection)