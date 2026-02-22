cmake_minimum_required(VERSION 3.10)
project(project_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(project_1 main.cpp
        aia_sort.h
        aia_sort.cpp
        aia_io.h
        aia_io.cpp)
