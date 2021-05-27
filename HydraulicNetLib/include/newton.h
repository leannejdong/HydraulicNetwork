#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
#include <cmath>
#include <cstdlib>

#include <Eigen/Dense>

using std::abs;

constexpr double K4{0.0268};
constexpr double K5{0.0064};
constexpr double K6{0.0021};
constexpr double K7{0.0268};
#ifndef HYDRAULICNETWORK_NEWTON_H
#define HYDRAULICNETWORK_NEWTON_H
void newton3d(Eigen::VectorXd& m,
              double tolerance,
              std::function<Eigen::VectorXd(const Eigen::VectorXd&)> F,
              std::function<Eigen::MatrixXd(const Eigen::VectorXd&)> DF
);
#endif//HYDRAULICNETWORK_NEWTON_H
