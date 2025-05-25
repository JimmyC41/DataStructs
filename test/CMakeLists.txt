find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests
    smart_ptr_test.cpp
    array_test.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(unit_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
)

enable_testing()