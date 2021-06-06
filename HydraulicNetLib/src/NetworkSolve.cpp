#include "NetworkSolve.h"
#include "Data.h"
#include "util.h"
//#include "newton.h"
#include <range/v3/view.hpp>
#include <typeinfo>
#include <fstream>
#include <string>
#include <vector>
#include "../include/inci.h"
#include "../include/util.h"

using Eigen::VectorXd;

void NetworkSolve()
{
    MatrixXd demand;
    demand = openData("inputs/Heating_demand_Liu.csv");

    //! Obtain massflow from energy
    VectorXd Demand_C = demand.col(0)/4.2/15;
    VectorXd Demand_D = demand.col(1)/4.2/15;
//    VectorXd Demand_E = demand.col(2)/4.2/15;
//    VectorXd Demand_F = demand.col(3)/4.2/15;
//    VectorXd Demand_G = demand.col(4)/4.2/15;

//    std::ofstream massflow("outputs/mflowLiu.csv");
//    massflow << "m1," << "m2," << "m3\n";
//    for(int i{0}; i < demand.rows(); ++i) {
//        //onst auto [m0, m1, m2, m3, m4, m5, m6] = newtonXd(Demand_C(i), Demand_D(i), Demand_E(i), Demand_F(i), Demand_G(i));
//        std::vector<double> solnVec = newtonXd(Demand_C(i), Demand_D(i)/*, Demand_E(i), Demand_F(i), Demand_G(i)*/);
//        //massflow << m0 << "," << m1 << "," << m2 << "," << m3 << "," << m4 << "," << m5 << "," << m6 << "\n";
//        for(auto&s : solnVec){
//            massflow << s << "," ;
//        }
//        massflow << "\n";



            //! get incidence matrix
    cerr << LoadFile().value_or("File [Network_info.csv] could not be opened!") << "\n";

//    std::vector<std::pair<std::string, std::vector<double>>> Network_info, Loop_info;
//        Network_info = read_csv("inputs/Network_info.csv");
//
//    const size_t N = (int)Network_info[0].second[0]; /// number of nodes
//    const size_t M = (int)Network_info[1].second[0]; /// number of pipes
//
//    vector<double> in = Network_info[3].second;
//    vector<double> out = Network_info[4].second;
//
//    int src = Network_info[5].second[0];
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
    MatrixXd A = makeEigenMatrixFromVectors(matA);
    cerr << "The matrix A is \n"
         << A << "\n";
    MatrixXd A_t = -A.transpose();

    removeRow(A_t, 2);
    cerr << "The final matrix -A^T is \n"
         << A_t << "\n";
//        VectorXd mass_flow(M);
//        mass_flow << 10.0, 10.0, 10.0;
//        std::vector<double> errors;
//        double tolerance = 1e-14;
//    Loop_info = read_csv("inputs/Pipes_info.csv");
//    std::vector<double> lengths, diameters, roughness;
//    lengths = Loop_info[1].second;
//    diameters = Loop_info[2].second;
//    roughness = Loop_info[3].second;
//    double initial_guess = Loop_info[4].second[0];


//    VectorXd reynolds(M), f(M), head(M);
//    int l = 1;
//    MatrixXd resistance; resistance(l, M);
//
//
//    for(int i = 0; i < 3; ++i){
//        reynolds(i) = 4*mass_flow(i)/mu/rho/diameters[i];
//        if(reynolds(i) < 2300){
//            f(i) = 64/reynolds(i);
//        } else {
//            f(i) = pow((1/-1.8/ log10((6.9/reynolds(i)) + roughness[i]/(diameters[i]/3.7))),2);
//        }
//        head(i) = 8*f(i)*lengths[i]/pow(diameters[i],5)/pow(rho, 2)/pow(PI,2)/9.81;
//    }

//    }

}
