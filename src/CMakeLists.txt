add_library(DataStructLib
    unique_ptr.cpp
    shared_ptr.cpp
)

target_include_directories(DataStructLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)