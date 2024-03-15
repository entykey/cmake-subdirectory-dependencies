cmake_minimum_required(VERSION 3.13)
project(MyProject)

add_subdirectory(A)
add_subdirectory(B)

add_executable(MyExecutable main.cpp)
target_link_libraries(MyExecutable PRIVATE A B)
