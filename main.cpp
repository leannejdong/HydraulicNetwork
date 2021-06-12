#include <exception>
#include <iostream>
//#include <optional>
#include <string>
#include <vector>
#include "../include/inci.h"
#include "../include/util.h"
#include "../include/NetworkSolve.h"

using Pair = std::vector<std::pair<int, int>>;
int main()
{
    //! get incidence matrix
    cerr << LoadFile().value_or("File [network1.csv] could not be opened!") << "\n";
    ifstream in("inputs/network1.csv");
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
    cerr << "The input data is parsed as "
         << "\n";
    for (auto &row : data) {
        for (auto &col : row) {
            cerr << col << " ";
        }
        cerr << "\n";
    }
    //! define the number of Nodes and Pipes
    const size_t n = stoi(data[0][0]);
    const size_t m = stoi(data[0][1]);
    //    std::cerr << "the number of nodes is " << n << "\n";
    //    std::cerr << "the number of pipes is " << m << "\n";

    vector<string> col4, col5;
    std::cerr << "the number of rows is " << data.size() << "\n";
    for (size_t i = 0; i < data.size(); ++i) {
        // std::cerr << data[i][3];
        col4.push_back(data[i][3]);
        col5.push_back(data[i][4]);
        //printf("Element %d: %d", i, data[i][0]);
    }

    std::cerr << "now let us convert vector of string of vector of ints"
              << "\n";
    vector<size_t> col4_int, col5_int;
    for (auto c4 : col4) {
        col4_int.push_back(stoi(c4));
    }
    //! c++ index from 0, so subtract 1 from each element of our vector
    for (auto &e4 : col4_int) {
        e4 -= 1;
    }

    for (auto c5 : col5) {
        col5_int.push_back(stoi(c5));
    }

    for (auto &e5 : col5_int) {
        e5 -= 1;
    }

    std::cerr << "column 4 is "
              << "\n";
    //    for(auto c4: col4_int){
    //        std::cerr << c4 << "\t";
    //    }
    //    std::cerr << "\n";
    printVec(col4_int);
    std::cerr << "column 5 is "
              << "\n";

    printVec(col5_int);
    vector<vector<int>> matA = gen_mat(m, n, col4_int, col5_int);
    //printMat(matA);
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
    for(size_t i{0}; i < col4_int.size(); ++i)
    {
        V.emplace_back(col4_int[i], col5_int[i]);
    }
    matrix adj(7);
    for(auto &v : V)
    {
        addEdge(adj, v.first, v.second);
    }
    cerr << "Print adjacency matrix: " << "\n";
    displayMatrix(adj);

    cerr << "Solving for mass flows: " << "\n";

    try {
        NetworkSolve();
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << '\n';
        throw;
    }


    return 0;
}
