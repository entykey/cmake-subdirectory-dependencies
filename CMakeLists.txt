cmake_minimum_required(VERSION 3.28)
project(MyProject)

add_subdirectory(A)
add_subdirectory(B)

add_executable(Target testingcpp23.cpp)
target_link_libraries(Target PRIVATE A B)


# without the below block, cmake won't recognize C++11 features, such as the namespace std::chrono_literals;, the use of auto in function parameters, print
# To use C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(Target PUBLIC cxx_std_23)


# Define ANSI escape codes for different colors
if (UNIX)
    string(ASCII 27 Esc)
    set(ColorReset "${Esc}[m")
    set(ColorRed "${Esc}[31m")
    set(ColorGreen "${Esc}[32m")
endif()

# Function to print colored message
function(print_color_message COLOR MESSAGE)
    message("${COLOR}${MESSAGE}${ColorReset}")
endfunction()

# Print the targeted C++ version
print_color_message("${ColorGreen}" "Targeted C++ version: C++${CMAKE_CXX_STANDARD}")
