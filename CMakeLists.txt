cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ecc-cpp)

add_executable(ecc-cpp include/math_operations.h src/math_operations.cpp main.cpp
        include/Curve.h
        include/Point.h
        src/Point.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)
target_link_libraries(ecc-cpp PkgConfig::GMPXX)
