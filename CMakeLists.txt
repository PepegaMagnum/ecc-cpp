cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ecc-cpp)

add_executable(ecc-cpp include/math_operations.h src/math_operations.cpp main.cpp
        src/Point.cpp
)