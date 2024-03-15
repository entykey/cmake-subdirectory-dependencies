# message(STATUS "CMake: B")
# set(A_DIR "${CMAKE_BINARY_DIR}/A")
# message(STATUS "CMake: B ${A_DIR}")
# find_package(A CONFIG )
# message(STATUS "---> ${A_FOUND}")
# add_library(B B.hpp B.cpp)
# target_link_libraries(B AA::A)



# CXX lib
# message(STATUS "CMake: B")

# add_library(B SHARED B.hpp B.cpp)
# target_include_directories(B PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


# C lib
message(STATUS "CMake: B")

add_library(B SHARED B.h B.c)
target_include_directories(B PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

