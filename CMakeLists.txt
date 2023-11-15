cmake_minimum_required(VERSION 3.23)
project(Different_partition_for_quick_sort C)

set(CMAKE_C_STANDARD 99)

add_executable(Different_partition_for_quick_sort main.c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Ofast -fno-rtti -fno-exceptions")
set(CMAKE_BUILD_TYPE Release)