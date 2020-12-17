cmake_minimum_required(VERSION 3.16)
project(omptest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(omptest main.cpp)