#include <iostream>
#include "A/A.hpp"
#include "B/B.h"

// We have A is a CXX project

// If B is a C project
// Forward declaration of the methodB() function from the B library
extern "C" void methodB();

int main() {
    A a_instance;

    std::cout << "Calling methods from A:\n";
    a_instance.methodA();

    std::cout << "\nCalling methods from B:\n";
    // Call methodB() from library B (C lib)
    methodB();

    return 0;
}


// If B is a CXX project
// int main() {
//     A a_instance;
//     B b_instance;

//     std::cout << "Calling methods from A:\n";
//     a_instance.methodA();

//     std::cout << "\nCalling methods from B:\n";
//     b_instance.methodB();

//     return 0;
// }
