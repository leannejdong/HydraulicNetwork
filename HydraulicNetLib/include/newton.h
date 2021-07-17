#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
#include <cmath>
#include <cstdlib>
#include <Eigen/Dense>
#include <fstream>
#include "util.h"
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::cerr;
constexpr double mu{4.04e-04};
constexpr double rho{980};
constexpr double PI{3.14159265};
const double tolerance = 1e-14;
constexpr int l = 10;
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H
MatrixXd newtonXd(MatrixXd &demands, VectorXd &consumers, MatrixXd &A_eigen_t, const size_t n, const size_t m)
{
    MatrixXd Loop_info;
    Loop_info = openData("inputs/Pipes_information.csv");
    VectorXd lengths = Loop_info.col(0);
    VectorXd diameters = Loop_info.col(1);
    VectorXd roughness = Loop_info.col(2)/1000;
/// initialization
    const double initial_guess = 1;
    VectorXd mass_flow = VectorXd::Zero(m);

    MatrixXd final_mass_flow= MatrixXd::Zero(demands.col(0).size(), m);

    VectorXd mass_flow_new = VectorXd::Zero(m);
    MatrixXd B_mat;
    B_mat = openData("inputs/Loops.csv");

//    std::ofstream jacob_mat("outputs/jacobian.csv");
//    std::ofstream res_mat("outputs/resistance.csv");
//    std::ofstream head_vec("outputs/head.csv");
//    std::ofstream force("outputs/force.csv");
//    std::ofstream rey("outputs/reynold.csv");
//    std::ofstream dia("outputs/diameters.csv");
//    // std::cout << mass_flow << "\n";

/// Initialize all array object to calculate mass flow rate

//    VectorXd reynolds = VectorXd::Zero(m);
//    VectorXd f = VectorXd::Zero(m);
//    VectorXd head = VectorXd::Zero(m);
//
//    MatrixXd resistance = MatrixXd::Zero(l, m);
//    //  std::cout << resistance;
//    MatrixXd F22 = MatrixXd::Zero(l, m);
//    int sum = 0; double err = 100;
//    VectorXd F2 = VectorXd::Zero(l);
//    std::ofstream jacob_mat("outputs/jacobian.csv");
//    std::ofstream res_mat("outputs/resistance.csv");
//    std::ofstream head_vec("outputs/head.csv");
//    std::ofstream force("outputs/force.csv");
//    std::ofstream rey("outputs/reynold.csv");
//    std::ofstream dia("outputs/diameters.csv");
//    // std::cout << mass_flow << "\n";
//
//    MatrixXd jacob(A_eigen_t.rows() + resistance.rows(), resistance.cols());
    for(int k{0}; k < demands.col(0).size(); ++k){
    //for(int k{0}; k < 2; ++k){
        mass_flow.setConstant(initial_guess);


        vector<double> external_flow(n);
/// Assigning node with proper external mass flow rate
        for(size_t i{0}; i < consumers.size(); ++i){
            for(size_t j{0}; j < n; ++j){
                if(consumers[i] - 1 == j){
                    external_flow[j] = demands(k,i);
                }
            }
        }
        //  printVec(external_flow);
        external_flow.erase(external_flow.begin()+148-1);
        //    printVec(external_flow);
  //      std::cerr << mass_flow << "\n";

/// Initialize all array object to calculate mass flow rate
        MatrixXd B_mat;
        B_mat = openData("inputs/Loops.csv");

        VectorXd reynolds = VectorXd::Zero(m);
        VectorXd f = VectorXd::Zero(m);
        VectorXd head = VectorXd::Zero(m);

        MatrixXd resistance = MatrixXd::Zero(l, m);
      //  std::cout << resistance;
        MatrixXd F22 = MatrixXd::Zero(l, m);
        int sum = 0; double err = 100;
        VectorXd F2 = VectorXd::Zero(l);
        std::ofstream jacob_mat("outputs/jacobian.csv");
        std::ofstream res_mat("outputs/resistance.csv");
        std::ofstream head_vec("outputs/head.csv");
        std::ofstream force("outputs/force.csv");
        std::ofstream rey("outputs/reynold.csv");
        std::ofstream dia("outputs/diameters.csv");
       // std::cout << mass_flow << "\n";

        MatrixXd jacob(A_eigen_t.rows() + resistance.rows(), resistance.cols());
        // cerr << "The size of the jacobien is " << jacob.rows() << "rows and " << jacob.cols() << "columns\n";
//        std::cout << mass_flow << "\n";
//        std::cout << mu << "and" << rho << "and" << PI << "\n";

        while(err > tolerance){
            //mass_flow.setConstant(initial_guess);
        //    std::cerr << mass_flow << "\n";
            for(size_t i = 0; i < m; ++i){  // calculate friction
                if(mass_flow.isZero(0.0)){
                    head.setConstant(0);
                    //head(i) = 0;
                } else{
                    //mass_flow.setConstant(initial_guess);
                    reynolds(i) = 4*abs(mass_flow(i))/mu/rho/PI/diameters(i);
                    //reynolds.array() = 4/mu/rho/PI/(diameters.array().abs());
                    if(reynolds(i) < 2300){
                        f(i) = 64/reynolds(i);
                    } else {
                        f(i) = pow((1/-1.8/ log10((6.9/reynolds(i)) + pow(roughness(i)/(diameters(i)/3.7),1.11))    ),2);
                    }
                    head(i) = 8*f(i)*lengths(i)/pow(diameters(i),5)/pow(rho, 2)/pow(PI,2)/9.81;
                }
            } // if the loop matrix is 0, the network is linear
            if(B_mat.isZero(0.0)) {
                VectorXd F(A_eigen_t.rows() + resistance.rows());
                F = A_eigen_t * mass_flow - Eigen::VectorXd::Map(external_flow.data(), external_flow.size() + 1);// F depends on conservation of mass and pressure
                mass_flow_new = mass_flow - A_eigen_t.inverse() * F;
                err = (mass_flow_new - mass_flow).norm() / mass_flow.norm();
                mass_flow = mass_flow_new;
            }
            else {
//                for(int i = 0; i < l; ++i) {
//                    for (size_t j = 0; j < m; ++j) {
//                        resistance(i, j) = 2 * B_mat(i, j) * head(j) * mass_flow(j);
//                        F22(i, j) = B_mat(i, j) * head(j) * mass_flow(j) * abs(mass_flow(j));
//                        sum += F22(i, j);
//                    }
//                    F2(i) = sum;
//                }
               // std::cerr << mass_flow << "\n";
                resistance = 2*B_mat*head.asDiagonal()*mass_flow.asDiagonal();
                //resistance = 2*B_mat*head*(mass_flow.transpose());
                F22  = B_mat * head.asDiagonal()*mass_flow.cwiseAbs2().asDiagonal();
                //F22  = B_mat * head *mass_flow.transpose()*mass_flow.cwiseAbs();
                F2 = F22.rowwise().sum();
             //   std::cerr << "The number of rows is " << resistance.rows() << "\n";
                jacob << A_eigen_t, resistance;

                VectorXd F1(A_eigen_t.rows());
                F1 = A_eigen_t * mass_flow - Eigen::VectorXd::Map(external_flow.data(), external_flow.size());
                VectorXd F(A_eigen_t.rows() + resistance.rows());
                F << F1, F2;
                VectorXd mass_flow_new(m);
                mass_flow_new = mass_flow - jacob.inverse() * F;
                err = (mass_flow_new - mass_flow).norm()/mass_flow.norm();
                mass_flow = mass_flow_new;
            }

//
//            cerr << "The solutions are : \n";
//            cerr << mass_flow.transpose() << "\n";
        }
        for(size_t i{0}; i < m; ++i){
            final_mass_flow(k, i)= mass_flow_new(i);
        }
        //final_mass_flow = mass_flow_new;

       // std::cerr << "The number of rows is " << resistance.rows() << "\n";
//        saveData(res_mat, resistance);
//        saveData(jacob_mat, jacob);
        saveData(head_vec, head );
        saveData(force, f);
        saveData(rey, reynolds);
        saveData(dia, diameters);
        saveData(res_mat, resistance);
        saveData(jacob_mat, jacob);


    }


    return final_mass_flow;
    //return mass_flow;


}
#endif//HYDRAULICNETWORK_NEWTON_H
