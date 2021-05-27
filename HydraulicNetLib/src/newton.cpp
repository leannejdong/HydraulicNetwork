#include "newton.h"

void newton3d(Eigen::VectorXd& m,
              double tolerance,
              std::function<Eigen::VectorXd(const Eigen::VectorXd&)> F,
              std::function<Eigen::MatrixXd(const Eigen::VectorXd&)> DF
)

{
    Eigen::VectorXd m_prev, s;

    std::vector<double> errors;
    errors.push_back((F(m)).norm());
    do	{
        m_prev = m;
        s = DF(m).lu().solve(F(m));
        m = m-s; // newton iteration
    } while (s.norm() > tolerance*m.norm());
}