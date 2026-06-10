cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ecc-cpp)

set(SOURCE_FILES main.cpp)
add_executable(RhoPollard_run ${SOURCE_FILES})

include_directories(RhoPollard_lib)
add_subdirectory(RhoPollard_lib)
add_subdirectory(test)

target_link_libraries(RhoPollard_run RhoPollard_lib)
