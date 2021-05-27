#include "inci.h"
#include "util.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <eigenData.h>
#include "../include/NetworkSolve.h"
#include "../include/newton.h"
using Eigen::VectorXd;


MatrixXd calculateSoln() {
    // get incidence matrix
    cerr << LoadFile().value_or("File [network1.csv] could not be opened!") << "\n";
    ifstream in("inputs/network1.csv");
    //    if (!in) {
    //        std::cout << "\nThe data file could not be opened!\n";
    //    }
    //    else {
    //        std::cout << "\nData file opened successfully!\n";
    //    }

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
    //    for(size_t i=0; i<data.size(); ++i)
    //    {
    //        for(int j=0; j<data[0].size(); j++)
    //            cerr<<data[i][j]<<" ";
    //        cerr<<"\n";
    //    }
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
    for (size_t i = 0; i < data.size(); i++) {
        // std::cerr << data[i][3];
        col4.push_back(data[i][3]);
        col5.push_back(data[i][4]);
        //printf("Element %d: %d", i, data[i][0]);
    }
    //    std::cerr << "column 4 is " << "\n";
    //    for(auto c4: col4){
    //        std::cerr << c4 << "\t";
    //    }
    //    std::cerr << "\n";
    //    std::cerr << "column 5 is " << "\n";
    //    for(auto c5: col5){
    //        std::cerr << c5 << "\t";
    //    }

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
    //    for(auto c5: col5_int){
    //        std::cerr << c5 << "\t";
    //    }
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

    //VectorXd Demand_z0(8760);
    VectorXd Demand_z1(8760);
    VectorXd Demand_C = demand.col(0);
    VectorXd Demand_D = demand.col(1);
    VectorXd Demand_E = demand.col(2);
    VectorXd Demand_F = demand.col(3);
    VectorXd Demand_G = demand.col(4);
    VectorXd z_loop(8760);
    Demand_z1.setZero();
    z_loop.setZero();
    MatrixXd M = MatrixXd::Zero(8760, 7);
    for(int i{0}; i < demand.rows(); ++i){
        Demand_C(i) = Demand_C(i) / 4.2 / 15;
        Demand_D(i) = Demand_D(i) / 4.2 / 15;
        Demand_E(i) = Demand_E(i) / 4.2 / 15;
        Demand_F(i) = Demand_F(i) / 4.2 / 15;
        Demand_G(i) = Demand_G(i) / 4.2 / 15;

        auto F = [&](const Eigen::VectorXd &m){
            Eigen::VectorXd res(7);
            res << m(0) - m(1) -m(2),
                    m(1) - Demand_C(i),
                    m(2) - m(3) - m(6) -Demand_D(i),
                    m(3) - m(4) - Demand_E(i),
                    m(4) - m(5) - Demand_F(i),
                    m(5) + m(6) - Demand_G(i);
                    K4*m(3)*std::abs(m(3)) + K5*m(4)*std::abs(m(4))
                    + K6*m(5)*std::abs(m(5)) - K7*m(6)*std::abs(m(6));
                    return res;
        };

        auto DF = [&](const Eigen::VectorXd &m){
            Eigen::MatrixXd J(7, 7);
            J << 1, -1, -1, 0, 0, 0, 0,
                    0, 1, 0, 0, 0, 0, 0,
                    0, 0, 1, -1, 0, 0, -1,
                    0, 0, 0, 1, -1, 0, 0,
                    0, 0, 0, 0, 1, -1, 0,
                    0, 0, 0, 0, 0, 1, 1,
                    0, 0, 0, 2*K4*std::abs(m(3)), 2*K5*std::abs(m(4)), 2*K6*std::abs(m(5)), 2*K7*std::abs(m(6));
            return J;
        };
        Eigen::VectorXd m(7);
        m << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;
        double tolerance = 1e-14;
        newton3d(m, tolerance, F, DF);

    }


//    MatrixXd B(8760, 7);
//    B << Demand_z0, Demand_z1, Demand_C, Demand_D, Demand_E, Demand_F, Demand_G;
//    VectorXd vec(7);
//    vec << 0, 0, 0, 1, 1, 1, -1;
//    A_tran.conservativeResize(A_tran.rows() + 1, NoChange);
//    A_tran.row(A_tran.rows() - 1) = vec;
//    cerr << "Here is the final A \n";
//    cerr << A_tran << "\n";
//    MatrixXd X = A_tran.lu().solve(B.transpose());
//    // MatrixXd X = A_eigen.colPivHouseholderQr().solve(B.transpose());
//    cerr << "Here is the first set of final solution for mass flow \n";
//    cerr << X.col(0) << ",";
//    cerr << "\n";
//    cerr << "The size of solution X is " << X.rows() << " rows and " << X.cols() << " columns\n";
//    return X;
}

//void NetworkSolve() {
//    auto result = calculateSoln();
//    saveData("outputs/flowQ.csv", result.transpose());
//}