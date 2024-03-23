
// #include <iostream>
#include <stdio.h>
#include "A/A.hpp"
#include "B/B.h"

// We have A is a CXX project

// If B is a C project
// Forward declaration of the methodB() function from the B library
extern "C" void methodB();

int main() {
    A a_instance;   // Note class only exists in CXX

    printf("Calling methods from A:\n");
    a_instance.methodA();

    printf("\nCalling methods from B:\n");
    // Call methodB() from library B (C lib)
    methodB();

    return 0;
}
