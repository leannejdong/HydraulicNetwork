#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
//#include "../include/inci.h"
//#include "../include/util.h"
#include "../include/NetworkSolve.h"
//#include "../include/graph.h"
//#include "../include/eigenData.h"
#include "../include/Timer.h"
//using Pair = std::vector<std::pair<int, int>>;
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
