cmake_minimum_required(VERSION 3.13)
project(ECE420)

find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Lab1)
add_subdirectory(Lab2)
add_subdirectory(Lab3)