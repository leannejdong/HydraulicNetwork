#include <exception>
#include <iostream>
#include "../include/NetworkSolve.h"
#include "../include/Timer.h"
int main()
{
    HydraulicNetwork::Timer t2;
    std::cerr << "Measure time taken for solving massflow\n";
    try {
        HydraulicNetwork::NetworkSolve();
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << '\n';
        throw;
    }

    return 0;
}
