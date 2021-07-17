#include "NetworkSolve.h"
#include "eigenData.h"
#include "newton.h"
#include "inci.h"
#include "util.h"
#include "graph.h"
#include "eigenData.h"
#include "Timer.h"
#include <range/v3/view.hpp>
#include <typeinfo>

using Eigen::VectorXd;
using Pair = std::vector<std::pair<int, int>>;

void NetworkSolve()
{
    //! get incidence matrix
    //   cerr << LoadFile().value_or("File [nw_verona.csv] could not be opened!") << "\n";
    ifstream in("inputs/verona.csv");
    if(!in){
        std::cerr << "Unable to open data\n";
        //return EXIT_FAILURE;
    } else{
        std::cerr << "\nData file opened successfully!\n";
    }
    vector<vector<string>> data;
    string line, word;

    while (getline(in, line)) {
        stringstream ss(line);
        vector<string> row;
        while (getline(ss, word, ',')) {
            row.push_back(word);
        }
        data.push_back(row);
    }
//    cerr << "The input data is parsed as "
//         << "\n";
//    for (auto &row : data) {
//        for (auto &col : row) {
//            std::cout << col << " ";
//        }
//        std::cout << "\n";
//    }
    //! define the number of Nodes and Pipes
    const size_t n = stoul(data[0][0]);
    const size_t m = stoul(data[0][1]);
    std::cerr << "the number of nodes is " << n << "\n";
    std::cerr << "the number of pipes is " << m << "\n";

    //! Source nodes
    const size_t source = stoul(data[0][5]);
    std::cerr << "the source node is " << source << "\n";

    vector<string> col4, col5;
    std::cerr << "the number of rows is " << m << "\n";
    for (size_t i = 0; i < m; ++i) {
        // std::cerr << data[i][3];
        col4.push_back(data[i][3]);
        col5.push_back(data[i][4]);
        //printf("Element %d: %d", i, data[i][0]);
    }

    //   std::cerr << "now let us convert vector of string of vector of ints\n";
    vector<size_t> col4_int, col5_int;
    for (auto c2 : col4) {
        col4_int.push_back(stoi(c2));
    }
    //! c++ index from 0, so subtract 1 from each element of our vector
    for (auto &e2 : col4_int) {
        e2 -= 1;
    }

    for (auto c3 : col5) {
        col5_int.push_back(stoi(c3));
    }

    for (auto &e5 : col5_int) {
        e5 -= 1;
    }

//    std::cerr << "column 4 is "
//              << "\n";
    //printVec(col4_int);
//    std::cerr << "column 5 is "
//              << "\n";

    //printVec(col5_int);
    std::cerr << "Measure time taken for generating incidence matrix\n";
    Timer t0;
    vector<vector<int>> matA = gen_mat(m, n, col4_int, col5_int);
    //   outputMat(matA);
    // vector<vector<int>> A = outputReduceMat(matA);
    MatrixXd A_eigen = makeEigenMatrixFromVectors(matA);
//    cerr << "The matrix A is \n"
//         << A_eigen << "\n";

    MatrixXd A_tran = -A_eigen.transpose();
//    removeRow(A_tran, source-1);
//    cerr << "The matrix -A^T is \n"
//         << A_tran << "\n";
//    std::ofstream filestr("outputs/inci_tran.csv");
//    saveData(filestr, A_tran);
    //! Loop detection
    //! First we need adjacency matrix
    Pair V;
    for(size_t i{0}; i < col4_int.size(); ++i)
    {
        V.emplace_back(col4_int[i], col5_int[i]);
    }
    graph g(150);
    for(auto &v : V){
        g.addEdge(v.first, v.second);
    }

//    cerr << "Print adjacency matrix: " << "\n";
//    g.printMat();

    //! apply gotlieb algorithm to detect loops
    std::vector<int> cycles;
    //! find cycles from the  adjacency matrix
    //! convert the matrix type adj to double vector
//    vector<vector<int>> adjVec = convertMatrix(adj);
//    adj.displayMatrixVec(adjVec);
    std::cerr << "Measure time taken for running Gotlieb algorithm for cycle detection\n";
    Timer t1;
    g.Gotlieb123(std::back_inserter(cycles));
    std::ofstream of("cycles.data");
    std::cout << "Print Cycles " << "\n";
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);
    std::ofstream filestr("outputs/inci_tran_reduced.csv");
    removeRow(A_tran, source -1);
    saveData(filestr, A_tran);

    MatrixXd demands;
    demands = openData("inputs/Demand_loads.csv");
    //std::cerr << demands.col(0).size() << "\n";
    VectorXd consumers = demands.row(0);
    removeRow(demands, 0);
    std::ofstream output_mass_flow("outputs/mflow_verona.csv");


    MatrixXd solution = newtonXd(demands, consumers, A_tran, n, m);
    saveData(output_mass_flow, solution);



//    //! Obtain massflow from energy
////    VectorXd Demand_C = demand.col(0)/*/4.2/15*/;
////    VectorXd Demand_D = demand.col(1)/*/4.2/15*/;
////    VectorXd Demand_E = demand.col(2)/*/4.2/15*/;
////    VectorXd Demand_F = demand.col(3)/*/4.2/15*/;
////    VectorXd Demand_G = demand.col(4)/*/4.2/15*/;
//
//
//    std::ofstream massflow("outputs/mflow.csv");
// //   massflow << "m1," << "m2," << "m3," << "m4," << "m5," << "m6," << "m7\n";
//    for(int i{0}; i < demand.rows(); ++i) {
//        //onst auto [m0, m1, m2, m3, m4, m5, m6] = newtonXd(Demand_C(i), Demand_D(i), Demand_E(i), Demand_F(i), Demand_G(i));
//        std::vector<double> solnVec = newtonXd(Demand_C(i), Demand_D(i), Demand_E(i), Demand_F(i), Demand_G(i));
//        //massflow << m0 << "," << m1 << "," << m2 << "," << m3 << "," << m4 << "," << m5 << "," << m6 << "\n";
//        for(auto&s : solnVec){
//            massflow << s << "," ;
//        }
//        massflow << "\n";
//
//    }

}
