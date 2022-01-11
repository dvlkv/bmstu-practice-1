cmake_minimum_required(VERSION 3.15)
project(kurs_1_praktika)

set(CMAKE_CXX_STANDARD 14)

add_executable(kurs_1_praktika main.cpp Menu.cpp Menu.h primitives/dynamic_array.h utils.hpp student.hpp)