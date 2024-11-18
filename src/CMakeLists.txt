cmake_minimum_required(VERSION 3.22.0)
project(Reflection)
set(CMAKE_CXX_STANDARD 17)

# 生成一个静态库
file(GLOB_RECURSE SRC *.cpp)
message("源文件包括 ${SRC}")
add_library(reflection SHARED ${SRC})