cmake_minimum_required(VERSION 3.6)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    greedy.cpp
    genetic_greedy.cpp)

add_executable(src ${SOURCE_FILES})