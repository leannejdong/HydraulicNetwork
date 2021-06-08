#include "NetworkSolve.h"
#include "Data.h"
#include "util.h"
//#include "newton.h"
//#include <range/v3/view.hpp>
#include <typeinfo>
#include <fstream>
#include <string>
#include <vector>
#include "../include/inci.h"
#include "../include/util.h"
#include <algorithm>
#include <cstdlib>

using Eigen::VectorXd;

void NetworkSolve()
{
            //! get incidence matrix
    cerr << LoadFile().value_or("File [Network_info.csv] could not be opened!") << "\n";

    std::ifstream nw("inputs/Network_info.csv");
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
    std::cerr << "the number of rows is " << data.size() << "\n";
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
    cerr << "The matrix A is \n"
         << A_eigen << "\n";
    MatrixXd A_eigen_t = -A_eigen.transpose();

    removeRow(A_eigen_t, 2);
    cerr << "The Eigen version of the final matrix -A^T is \n"
         << A_eigen_t << "\n";


    vector<int> consumers{1, 2};
    //consumers << 1, 2;
    constexpr double setpoint_T{25};
    constexpr double inlet_T{40};

    MatrixXd demand_data, demands;
    demand_data = openData("inputs/Heating_demand_Liu.csv");
    demands = demand_data/4.2/(inlet_T - setpoint_T);

    //VectorXd external_flow(n); //?
    vector<double> external_flow(n);

    MatrixXd Loop_info;
    Loop_info = openData("inputs/Pipes_info.csv");
//    removeRow(Loop_info, 0);
//    removeColumn(Loop_info, 0);

    VectorXd lengths = Loop_info.col(0);
    VectorXd diameters = Loop_info.col(1);
    VectorXd roughness = Loop_info.col(2);

    double initial_guess = /*Loop_info(0,3);*/ 1;
    VectorXd mass_flow(m);
    mass_flow.setConstant(initial_guess);

    for(size_t i{0}; i < consumers.size(); ++i){
        for(size_t j{0}; j < n; ++j){
            if(consumers[i]-1 == j){
                external_flow[j] = demands(0,i);
            }
        }
    }
    external_flow.erase(external_flow.begin()+2);

    // how to do external_flow(3)=[];?
    VectorXd reynolds(m);
    reynolds.setZero();
    VectorXd f(m);
    f.setZero();
    VectorXd head(m);
    head.setZero();

    const double tolerance = 1e-14;
    constexpr int l = 1;

    MatrixXd resistance(l, m);
    MatrixXd F22(l, m);
    int sum = 0; double err = 100;
    VectorXd F2(l);
    F2.setZero();
    MatrixXd jacob(A_eigen_t.rows() + resistance.rows(), resistance.cols());
    MatrixXd B_mat = openData("inputs/Loops_Liu.csv");
    constexpr double mu = 0.294e-6;
    constexpr double rho = 997;
//
//
    std::ofstream output_mass_flow("outputs/mflowLiu_new");
    output_mass_flow << "m1," << "m2," << "m3\n";
    while(err > tolerance){
        for(int i = 0; i < 3; ++i){
            reynolds(i) = 4*mass_flow(i)/mu/rho/diameters(i);
            if(reynolds(i) < 2300){
                f(i) = 64/reynolds(i);
            } else {
                f(i) = pow((1/-1.8/ log10((6.9/reynolds(i)) + roughness(i)/(diameters(i)/3.7))),2);
            }
            head(i) = 8*f(i)*lengths(i)/pow(diameters(i),5)/pow(rho, 2)/pow(M_PI,2)/9.81;
        }
        for(int i = 0; i < l; ++i){
            for(size_t j = 0; j < m; ++j){
                resistance(i, j) = 2*B_mat(i, j)*head(j)*mass_flow(j);
                F22(i, j) = B_mat(i, j)*head(j)*mass_flow(j)*std::abs(mass_flow(j));
                sum += F22(i, j);
            }
            F2(i) = sum;
        }

        jacob << A_eigen_t, resistance;
        VectorXd F1(A_eigen_t.rows());
        F1 = A_eigen_t * mass_flow - Eigen::VectorXd::Map(external_flow.data(), external_flow.size());
        VectorXd F(A_eigen_t.rows()+1);
        F << F1, F2;
        VectorXd mass_flow_new(m);
        mass_flow_new = mass_flow - jacob.inverse() * F;
        err = (mass_flow_new - mass_flow).norm()/mass_flow.norm();
        mass_flow = mass_flow_new;


        saveData(output_mass_flow, mass_flow);
        cerr << mass_flow << " End\n";
    }

}
