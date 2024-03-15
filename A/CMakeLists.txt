message(STATUS "CMake: A")

add_library(A SHARED A.hpp A.cpp)
target_include_directories(A PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
