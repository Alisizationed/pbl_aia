cmake_minimum_required(VERSION 4.1)
project(project_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(project_1 main.cpp
        aia_sort.h
        aia_sort.cpp
        aia_io.h
        aia_io.cpp)
