#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
#include <cmath>
#include <cstdlib>
#include <Eigen/Dense>
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::cerr;
constexpr double K4{0.0268};
constexpr double K5{0.0064};
constexpr double K6{0.0021};
constexpr double K7{0.0268};
constexpr double mu{0.294e-6};
constexpr double rho{997};
constexpr double friction{0};
constexpr double PI{3.14159265};
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H

template<typename T>
//auto newtonXd(auto &dC, auto &dD, auto &dE, auto &dF, auto &dG)
//std::tuple<double, double, double, double, double, double, double> newtonXd(T &dC, T &dD, T &dE, T &dF, T &dG)
std::vector<double> newtonXd(T &dC, T &dD/*, T &dE, T &dF, T &dG*/)
{

    MatrixXd B_mat/*, Loop_info, Network_info*/;
    std::vector<std::pair<std::string, std::vector<double>>> Network_info, Loop_info;
    Network_info = read_csv("inputs/Network_info.csv");

    int N = (int)Network_info[0].second[0]; /// number of nodes
    int M = (int)Network_info[1].second[0]; /// number of pipes

    vector<double> in = Network_info[3].second;
    vector<double> out = Network_info[4].second;

//    int src = Network_info(1, 5);
    int src = Network_info[5].second[0];

    VectorXd mass_flow(M);
    mass_flow << 10.0, 10.0, 10.0;
    std::vector<double> errors;
    double tolerance = 1e-14;


    B_mat = openData("inputs/Loops_Liu.csv");
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



    auto F = [&](const Eigen::VectorXd &m){
      Eigen::VectorXd res(3);
      res << m(0) - m(1) - dC,
              m(1) + m(2) - dD,
      /*head(0)**/B_mat(0,0)*m(0)*std::abs(m(0)) + /*head(1)**/B_mat(0,1)*m(1)*std::abs(m(1))
      + /*head(2)**/B_mat(0,2)*m(2)*std::abs(m(2)) ;
//              m(0)*std::abs(m(0)) + m(1)*std::abs(m(1))
//     + m(2)*std::abs(m(2)) ;
      return res;
    };

    auto DF = [&](const Eigen::VectorXd &m){
      Eigen::MatrixXd J(3, 3);
      J << 1, -1, 0,
              0, 1, 1,
              /*head(0)**/2*B_mat(0,0)*std::abs(m(0)), /*head(1)**/2*B_mat(0,1)*std::abs(m(0)),
               /*head(2)**/2*B_mat(0, 2)*std::abs(m(2));
//              2*std::abs(m(0)), 2*std::abs(m(0)),
//              2*std::abs(m(2));
      return J;
    };

    Eigen::VectorXd m_prev, s;

    errors.push_back((F(mass_flow)).norm());
    do	{
        m_prev = mass_flow;
        s = DF(mass_flow).lu().solve(F(mass_flow));
        mass_flow = mass_flow-s; // newton iteration
    } while (s.norm() > tolerance* mass_flow .norm());

    //return std::make_tuple(m(0), m(1), m(2), m(3), m(4), m(5), m(6));
    //return std::vector<double>{mass_flow(0), mass_flow(1), mass_flow(2)/*, mass_flow(3), mass_flow(4), mass_flow(5), mass_flow(6)*/};
    return std::vector<double>{mass_flow(0), mass_flow(1), mass_flow(2)};


}

#endif//HYDRAULICNETWORK_NEWTON_H
