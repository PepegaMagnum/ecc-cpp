find_package(Catch2 3 REQUIRED)
add_executable(Catch2_tests_run CurveTests.cpp)

target_link_libraries(Catch2_tests_run PRIVATE RhoPollard_lib)
target_link_libraries(Catch2_tests_run PRIVATE Catch2::Catch2WithMain)


include(Catch)
catch_discover_tests(Catch2_tests_run)