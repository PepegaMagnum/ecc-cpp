cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ecc-cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops")

set(SOURCE_FILES main.cpp)
add_executable(RhoPollard_run ${SOURCE_FILES})

include_directories(RhoPollard_lib)
add_subdirectory(RhoPollard_lib)
add_subdirectory(test)

target_link_libraries(RhoPollard_run RhoPollard_lib)
