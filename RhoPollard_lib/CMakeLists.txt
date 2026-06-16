project(RhoPollard)

set(HEADER_FILES include/math_operations.h
                 include/Point.h
                 include/Curve.h
                 include/RhoPollard.h
)

set(SOURCE_FILES src/math_operations.cpp
                 src/Point.cpp
                 src/Curve.cpp
                 src/RhoPollard.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)

add_library(RhoPollard_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(RhoPollard_lib PUBLIC include)
target_link_libraries(RhoPollard_lib PRIVATE PkgConfig::GMPXX)