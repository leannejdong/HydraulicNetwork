#include "NetworkSolve.h"
#include "Data.h"
#include "util.h"
#include "newton.h"
//#include <range/v3/view.hpp>
#include <typeinfo>
//#include <fstream>
#include <string>
#include <vector>
//#include "../include/inci.h"
//#include "../include/util.h"
#include <algorithm>
#include <cstdlib>

using Eigen::VectorXd;



void NetworkSolve()
{

    //! get incidence matrix
    cerr << LoadFile().value_or("File [Network_information.csv] could not be opened!") << "\n";

    std::ifstream nw("inputs/Network_information.csv");
    vector<vector<string>> data;
    string line, word;

    while (std::getline(nw, line)) {
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
            cerr << col << ",";
        }
        cerr << "\n";
    }
    //! define the number of Nodes and Pipes
    const size_t n = stoi(data[1][0]);
    const size_t m = stoi(data[1][1]);
    std::cerr << "the number of nodes is " << n << "\n";
    std::cerr << "the number of pipes is " << m << "\n";

    vector<string> col4, col5;
    std::cerr << "the number of columns is " << data.size() << "\n";
    for (size_t i = 1; i < data.size(); ++i) {
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
    cerr << "The matrix A is \n";
    printMat(matA);
    // vector<vector<int>> matA_t = mat_tran(matA);
    // vector<vector<int>> A = outputReduceMat(matA);
    MatrixXd A_eigen = makeEigenMatrixFromVectors(matA);
    cerr << "The Eigen version of matrix A is \n"
         << A_eigen << "\n";
    MatrixXd A_eigen_t = -A_eigen.transpose();

    removeRow(A_eigen_t, 0); // remove the row correspond to the source node
    cerr << "The Eigen version of the final matrix -A^T is \n"
         << A_eigen_t << "\n";


    vector<double> consumers{3, 4};

    /*! Parse csv as Eigen matrix formatted objects */
    MatrixXd demands = openData("inputs/Demand_loads.csv");
    //demands = demand_data;
    std::cerr << demands.col(0).size() << "\n";

    //! Create oftsream for writing the solutions to file
    std::ofstream output_mass_flow("outputs/mflowLiu_1606.csv");
    output_mass_flow << "m1," << "m2," << "m3\n";
    //MatrixXd solution = MatrixXd::Zero(demands.col(0).size(), m);
    //! Involve the solver to find solutions, given the demands matrix, cusumer vector, incidence matrix, num_nodes, num_edges
    MatrixXd solution = newtonXd(demands, consumers, A_eigen_t, n, m);
    saveData(output_mass_flow, solution);




}
