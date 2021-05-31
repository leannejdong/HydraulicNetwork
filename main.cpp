#include "inci.h"
#include "util.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <eigenData.h>
#include "../include/newton.h"


int main()
{
    // get incidence matrix
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
    // define the number of Nodes and Pipes
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
    // c++ index from 0, so subtract 1 from each element of our vector
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


    MatrixXd demand;
    demand = openData("inputs/Heating_demand.csv");

    //! Obtain massflow from energy
    VectorXd Demand_C = demand.col(0)/4.2/15;
    VectorXd Demand_D = demand.col(1)/4.2/15;
    VectorXd Demand_E = demand.col(2)/4.2/15;
    VectorXd Demand_F = demand.col(3)/4.2/15;
    VectorXd Demand_G = demand.col(4)/4.2/15;

    std::ofstream massflow("outputs/mflow.csv");
    massflow << "m1," << "m2," << "m3," << "m4," << "m5," << "m6," << "m7\n";
    for(int i{0}; i < demand.rows(); ++i){
        auto[m0, m1, m2, m3, m4, m5, m6] = newtonXd(Demand_C(i), Demand_D(i), Demand_E(i), Demand_F(i), Demand_G(i));
        massflow << m0 << "," << m1 << "," << m2 << "," << m3 << "," << m4 << "," << m5 << "," << m6 <<"\n";
    }


    //saveData(file, "outputs/flowQ_test.csv", m0.transpose());

    return 0;
}
