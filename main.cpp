#include <exception>
#include <iostream>

#include "../include/NetworkSolve.h"

int main()
{
    try {
        NetworkSolve(); // test for errors
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << '\n';
        throw; // when error detected the program throws us an error

    }
    return 0;
}
