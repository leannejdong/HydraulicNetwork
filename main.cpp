#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "../include/inci.h"
#include "../include/util.h"
#include "../include/NetworkSolve.h"
#include "../include/graph.h"
using Pair = std::vector<std::pair<int, int>>;
int main()
{
    //! get incidence matrix
 //   cerr << LoadFile().value_or("File [nw_verona.csv] could not be opened!") << "\n";
 //   ifstream in("inputs/nw_verona.csv");
    ifstream in("inputs/nw4-3.csv");
 //   ifstream in("inputs/network1.csv");
    if(!in){
        std::cerr << "Unable to open data\n";
        return EXIT_FAILURE;
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
//    const size_t n = stoi(data[0][0]);
//    const size_t m = stoi(data[0][1]);
    const size_t n = 4;
    const size_t m = 3;
//    const size_t n = 4;
//    const size_t m = data.size();
        std::cerr << "the number of nodes is " << n << "\n";
        std::cerr << "the number of pipes is " << m << "\n";

    vector<string> col2, col3;
    std::cerr << "the number of rows is " << data.size() << "\n";
    for (size_t i = 0; i < data.size(); ++i) {
        // std::cerr << data[i][3];
        col2.push_back(data[i][1]);
        col3.push_back(data[i][2]);
        //printf("Element %d: %d", i, data[i][0]);
    }

    std::cerr << "now let us convert vector of string of vector of ints"
              << "\n";
    vector<size_t> col2_int, col3_int;
    for (auto c2 : col2) {
        col2_int.push_back(stoi(c2));
    }
    //! c++ index from 0, so subtract 1 from each element of our vector
    for (auto &e2 : col2_int) {
        e2 -= 1;
    }

    for (auto c3 : col3) {
        col3_int.push_back(stoi(c3));
    }

    for (auto &e5 : col3_int) {
        e5 -= 1;
    }

    std::cerr << "column 4 is "
              << "\n";
    //    for(auto c4: col4_int){
    //        std::cerr << c4 << "\t";
    //    }
    //    std::cerr << "\n";
    printVec(col2_int);
    std::cerr << "column 5 is "
              << "\n";

    printVec(col3_int);
    vector<vector<int>> matA = gen_mat(m, n, col2_int, col3_int);
    printMat(matA);
    // vector<vector<int>> A = outputReduceMat(matA);
    MatrixXd A_eigen = makeEigenMatrixFromVectors(matA);
    cerr << "The matrix A is \n"
         << A_eigen << "\n";

    MatrixXd A_tran = -A_eigen.transpose();
    removeRow(A_tran, 0);
    cerr << "The matrix -A^T is \n"
         << A_tran << "\n";
    /// Loop detection
    /// First we need adjacency matrix
    Pair V;
    for(size_t i{0}; i < col2_int.size(); ++i)
    {
        V.emplace_back(col2_int[i], col3_int[i]);
    }
    graph g(150);
    for(auto &v : V){
        g.addEdge(v.first, v.second);
    }

    cerr << "Print adjacency matrix: " << "\n";
    g.printMat();

    /// apply gotlieb algorithm to detect loops
    std::vector<int> cycles;
    /// find cycles from the  adjacency matrix
    /// convert the matrix type adj to double vector
//    vector<vector<int>> adjVec = convertMatrix(adj);
//    adj.displayMatrixVec(adjVec);
    g.Gotlieb123(std::back_inserter(cycles));
    std::ofstream of("cycles.data");
    std::cout << "Print Cycles " << "\n";
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);

    cerr << "Solving for mass flows: " << "\n";

    try {
        NetworkSolve();
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << '\n';
        throw;
    }


    return 0;
}
