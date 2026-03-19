cmake_minimum_required(VERSION 4.1)
project(project_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(project_2 main.cpp
        sssp_algorithms.h
        sssp_algorithms.cpp
        sssp_io.cpp
        sssp_io.h)
