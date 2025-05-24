cmake_minimum_required(VERSION 4.0.2)
set(CMAKE_CXX_STANDARD 23)

project(DataStructs)

add_executable(dummy main.cpp)

add_subdirectory(src)

target_link_libraries(dummy PRIVATE DataStructLib)

add_subdirectory(test)