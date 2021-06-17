#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
#include <cmath>
#include <cstdlib>
#include <Eigen/Dense>
#include <fstream>
#include "inci.h"
#include "util.h"
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::cerr;

constexpr double mu{4.04e-04};
constexpr double rho{980};
constexpr double PI{3.14159265};
const double tolerance = 1e-14;
constexpr int l = 1;
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H

MatrixXd newtonXd(MatrixXd &demands, vector<double> &consumers, MatrixXd &A_eigen_t, const size_t n, const size_t m)
{
    MatrixXd final_mass_flow= MatrixXd::Zero(demands.col(0).size(), m);

    demands = demands*rho/3600;

    MatrixXd Loop_info;
    Loop_info = openData("inputs/Pipes_information.csv");
//    removeRow(Loop_info, 0);
//    removeColumn(Loop_info, 0);

    VectorXd lengths = Loop_info.col(0);
    VectorXd diameters = Loop_info.col(1);
    VectorXd roughness = Loop_info.col(2);

    double initial_guess = /*Loop_info(0,3);*/ 1;
    VectorXd mass_flow(m);
    mass_flow.setConstant(initial_guess);
    VectorXd mass_flow_new = VectorXd::Zero(m);

    for(int k{0}; k < demands.col(0).size(); ++k){

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
        external_flow.erase(external_flow.begin(), external_flow.begin()+2);
    //    printVec(external_flow);

/// Initialize all array object to calculate mass flow rate
        MatrixXd B_mat;
        B_mat = openData("inputs/Loops.csv");

        // how to do external_flow(3)=[];?
        VectorXd reynolds = VectorXd::Zero(m);
        // reynolds.setZero();
        VectorXd f = VectorXd::Zero(m);
        // f.setZero();
        VectorXd head = VectorXd::Zero(m);
        // head.setZero();

        MatrixXd resistance(l, m);
        MatrixXd F22(l, m);
        int sum = 0; double err = 100;
        VectorXd F2(l);
        F2.setZero();
        MatrixXd jacob(A_eigen_t.rows() + resistance.rows(), resistance.cols());

        while(err > tolerance){
            for(size_t i = 0; i < m; ++i){
                reynolds(i) = 4*mass_flow(i)/mu/rho/diameters(i);
                if(reynolds(i) < 2300){
                    f(i) = 64/reynolds(i);
                } else {
                    f(i) = pow((1/-1.8/ log10((6.9/reynolds(i)) + pow(roughness(i)/(diameters(i)/3.7),1.11))    ),2);
                }
                head(i) = 8*f(i)*lengths(i)/pow(diameters(i),5)/pow(rho, 2)/pow(PI,2)/9.81;
            } // if the loop matrix is 0, the network is linear
            if(B_mat.isZero(0.0)) {
                VectorXd F(A_eigen_t.rows() + resistance.rows());
                F = A_eigen_t * mass_flow - Eigen::VectorXd::Map(external_flow.data(), external_flow.size() + 1);
                mass_flow_new = mass_flow - A_eigen_t.inverse() * F;
                err = (mass_flow_new - mass_flow).norm() / mass_flow.norm();
                mass_flow = mass_flow_new;
            }
            else {
                for(int i = 0; i < l; ++i) {
                    for (size_t j = 0; j < m; ++j) {
                        resistance(i, j) = 2 * B_mat(i, j) * head(j) * mass_flow(j);
                        F22(i, j) = B_mat(i, j) * head(j) * mass_flow(j) * std::abs(mass_flow(j));
                        sum += F22(i, j);
                    }
                    F2(i) = sum;
                }
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

    }

    return final_mass_flow;


}

#endif//HYDRAULICNETWORK_NEWTON_H
