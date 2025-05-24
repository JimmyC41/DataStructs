cmake_minimum_required(VERSION 4.0.2)

project(DataStructs)

add_executable(dummy main.cpp)

add_subdirectory(src)

target_link_libraries(dummy PRIVATE DataStructLib)