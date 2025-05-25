cmake_minimum_required(VERSION 4.0.2)
set(CMAKE_CXX_STANDARD 23)

project(DataStructs)

# add_subdirectory(src)

add_executable(dummy main.cpp)

target_include_directories(dummy
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(dummy
    PRIVATE
        # DataStructLib
)

add_subdirectory(test)