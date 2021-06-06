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

    MatrixXd B_mat;
    B_mat = openData("inputs/Loops_Liu.csv");

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
