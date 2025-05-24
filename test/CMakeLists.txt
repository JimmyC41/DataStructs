find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests smart_ptr_test.cpp)

target_link_libraries(unit_tests
    PRIVATE
        DataStructLib
        GTest::gtest
        GTest::gtest_main
)

enable_testing()