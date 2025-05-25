add_library(DataStructLib
    unique_ptr.cpp
    shared_ptr.cpp
    array.cpp
)

target_include_directories(DataStructLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)