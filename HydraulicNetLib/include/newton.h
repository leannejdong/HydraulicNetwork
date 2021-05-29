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
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H

auto newtonXd(auto &dC, auto &dD, auto &dE, auto &dF, auto &dG)
{
    Eigen::VectorXd m(7);
    m << 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0;
    double tolerance = 1e-14;

    auto F = [&](const Eigen::VectorXd &m){
      Eigen::VectorXd res(7);
      res << m(0) - m(1) -m(2),
              m(1) - dC,
              m(2) - m(3) - m(6) - dD,
              m(3) - m(4) - dE,
              m(4) - m(5) - dF,
              m(5) + m(6) - dG,
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

    Eigen::VectorXd m_prev, s;

    std::vector<double> errors;
    errors.push_back((F(m)).norm());
    do	{
        m_prev = m;
        s = DF(m).lu().solve(F(m));
        m = m-s; // newton iteration
    } while (s.norm() > tolerance*m.norm());

    return std::make_tuple(m(0), m(1), m(2), m(3), m(4), m(5), m(6));

}

#endif//HYDRAULICNETWORK_NEWTON_H
