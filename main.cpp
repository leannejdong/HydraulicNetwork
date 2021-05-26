#include "inci.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <eigenData.h>

using Eigen::VectorXd;

int main()
{
//    //const auto [exists, s] = LoadFile();
//    const auto s = LoadFile();
//   // if( /*s.empty()*/!exists)
//    if(s)
//    {
//        //cerr << "File [test.txt] could not be opened!\n";
//        cerr << *s << "\n";
//    }else{
//        //cerr << s << "\n";
//        cerr << "File [test.txt] could not be opened!\n";
//    }
// with std::optional, we need indeed just one line
    cerr << LoadFile().value_or("File [network1.csv] could not be opened!") << "\n";
    ifstream in("inputs/network1.csv");
//    if (!in) {
//        std::cout << "\nThe data file could not be opened!\n";
//    }
//    else {
//        std::cout << "\nData file opened successfully!\n";
//    }

    vector<vector<string> > data;
    string line, word;

    while (getline(in,line))
    {
        stringstream ss(line);
        vector<string> row;
        while (getline(ss, word, ','))
        {
            row.push_back(word);
        }
        data.push_back(row);
    }
    cerr << "The input data is parsed as " << "\n";
//    for(size_t i=0; i<data.size(); ++i)
//    {
//        for(int j=0; j<data[0].size(); j++)
//            cerr<<data[i][j]<<" ";
//        cerr<<"\n";
//    }
    for(auto &row : data){
        for(auto &col : row){
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
    for(size_t i = 0; i < data.size(); i++)
    {
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

    std::cerr << "now let us convert vector of string of vector of ints" << "\n";
    vector<size_t> col4_int, col5_int;
    for(auto c4 : col4){
        col4_int.push_back(stoi(c4));
    }
    // c++ index from 0, so subtract 1 from each element of our vector
    for(auto & e4 : col4_int)
    {
        e4 -= 1;
    }

    for(auto c5 : col5){
        col5_int.push_back(stoi(c5));
    }

    for(auto & e5 : col5_int)
    {
        e5 -= 1;
    }

    std::cerr << "column 4 is " << "\n";
//    for(auto c4: col4_int){
//        std::cerr << c4 << "\t";
//    }
//    std::cerr << "\n";
    printVec(col4_int);
    std::cerr << "column 5 is " << "\n";
//    for(auto c5: col5_int){
//        std::cerr << c5 << "\t";
//    }
    printVec(col5_int);
    vector<vector<int>> matA = gen_mat(m, n, col4_int, col5_int);
    //printMat(matA);
    outputMat(matA);
    vector<vector<int>> A = outputReduceMat(matA);
    MatrixXd A_eigen = makeEigenMatrixFromVectors(A);
    MatrixXd A_tran = - A_eigen.transpose();
    //cerr << "The first row is " << A_eigen.row(0);
    MatrixXd demand;
    demand = openData("inputs/Heating_demand.csv");
    VectorXd Demand_z0(8760);
    VectorXd Demand_C = demand.col(0);
    VectorXd Demand_D = demand.col(1);
    VectorXd Demand_E = demand.col(2);
    VectorXd Demand_F = demand.col(3);
    VectorXd Demand_G = demand.col(4);
    Demand_z0.setZero();

    Demand_C = Demand_C/4.2/15;
    Demand_D = Demand_D/4.2/15;
    Demand_E = Demand_E/4.2/15;
    Demand_F = Demand_F/4.2/15;
    Demand_G = Demand_G/4.2/15;

    MatrixXd B(8760, 6);
    B << Demand_z0, Demand_C, Demand_D, Demand_E, Demand_F, Demand_G;
    VectorXd vec(7);
    vec << 0, 0, 0, 1, 1, 1, -1;
    A_eigen.conservativeResize(A_eigen.rows()+1, NoChange);
    A_eigen.row(A_eigen.rows() - 1) = vec;
    cerr << "Here is the final A \n" ;
    cerr << A_eigen << "\n";
    //MatrixXd X = A_eigen.lu().solve(B.transpose());
   // MatrixXd X = A_eigen.colPivHouseholderQr().solve(B.transpose());
    return 0;
}
