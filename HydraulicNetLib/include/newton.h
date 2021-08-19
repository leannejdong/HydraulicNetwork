#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
//#include <cmath>
#include <cstdlib>
#include <Eigen/Dense>
#include <fstream>
#include "util.h"
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::cerr;
constexpr long double mu{4.04e-04};
constexpr long double rho{980};
constexpr long double PI{3.14159265};
constexpr long double tolerance{1e-14};
constexpr int l{10};
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H
MatrixXd newtonXd(MatrixXd &demands, VectorXd &consumers, MatrixXd &A_eigen_t, const size_t n, const size_t m)
{
    MatrixXd Loop_info;
    Loop_info = HydraulicNetwork::openData("data/Pipes_information.csv");
    VectorXd lengths = Loop_info.col(0);
    VectorXd diameters = Loop_info.col(1);
    VectorXd roughness = Loop_info.col(2)/1000;
/// initialization
    const double initial_guess = 0.1;
    VectorXd mass_flow(m);

    MatrixXd final_mass_flow= MatrixXd::Zero(demands.col(0).size(), m);

    VectorXd mass_flow_new = VectorXd::Zero(m);
    MatrixXd B_mat;
    B_mat = HydraulicNetwork::openData("data/Loops.csv");

    for(int k{0}; k < demands.col(0).size(); ++k){
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
        external_flow.erase(external_flow.begin()+148-1);

/// Initialize all array object to calculate mass flow rate
        MatrixXd B_mat;
        B_mat = HydraulicNetwork::openData("data/Loops.csv");

        VectorXd reynolds = VectorXd::Zero(m);
        VectorXd f = VectorXd::Zero(m);
        VectorXd head = VectorXd::Zero(m);

        MatrixXd resistance = MatrixXd::Zero(l, m);
      //  std::cout << resistance;
        MatrixXd F22 = MatrixXd::Zero(l, m);
        int sum = 0; double err = 100;
        VectorXd F2 = VectorXd::Zero(l);
        std::ofstream jacob_mat("data/outputs/jacobian.csv");
        std::ofstream res_mat("data/outputs/resistance.csv");
        std::ofstream head_vec("data/outputs/head.csv");
        std::ofstream force("data/outputs/force.csv");
        std::ofstream rey("data/outputs/reynold.csv");
        std::ofstream dia("data/outputs/diameters.csv");
        std::ofstream error("data/outputs/error.csv");

        MatrixXd jacob(A_eigen_t.rows() + resistance.rows(), resistance.cols());
        while(err > tolerance){
            for(size_t i = 0; i < m; ++i){  // calculate friction
                if(mass_flow(i) == 0.00){
                    break;
                } else{
                    reynolds(i) = 4*std::abs(mass_flow(i))/mu/rho/PI/diameters(i);
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
                resistance = 2*B_mat*head.asDiagonal()*mass_flow.asDiagonal();
                F22  = B_mat * head.asDiagonal()*mass_flow.cwiseAbs2().asDiagonal();
                F2 = F22.rowwise().sum();
                jacob << A_eigen_t, resistance;

                VectorXd F1(A_eigen_t.rows());
                F1 = A_eigen_t * mass_flow - Eigen::VectorXd::Map(external_flow.data(), external_flow.size());
                VectorXd F(A_eigen_t.rows() + resistance.rows());
                F << F1, F2;
                mass_flow_new = mass_flow - jacob.inverse() * F;
                err = (mass_flow_new - mass_flow).norm()/mass_flow.norm();
                mass_flow = mass_flow_new;

                // std::cerr << mass_flow << " ";
            }
            error << err << ", ";
            HydraulicNetwork::saveData(res_mat, resistance);


        }
        for(size_t i{0}; i < m; ++i){
            final_mass_flow(k, i)= mass_flow_new(i);
        }

        HydraulicNetwork::saveData(head_vec, head );
        HydraulicNetwork::saveData(force, f);
        HydraulicNetwork::saveData(rey, reynolds);
        HydraulicNetwork::saveData(dia, diameters);
        HydraulicNetwork::saveData(jacob_mat, jacob);
    }


    return final_mass_flow;


}
#endif//HYDRAULICNETWORK_NEWTON_H
