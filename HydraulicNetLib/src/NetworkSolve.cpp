#include "NetworkSolve.h"
#include "eigenData.h"
#include "newton.h"
#include <range/v3/view.hpp>
#include <typeinfo>

using Eigen::VectorXd;

void NetworkSolve()
{
    MatrixXd demand;
    demand = openData("inputs/Heating_demand.csv");

    //! Obtain massflow from energy
    VectorXd Demand_C = demand.col(0)/4.2/15;
    VectorXd Demand_D = demand.col(1)/4.2/15;
    VectorXd Demand_E = demand.col(2)/4.2/15;
    VectorXd Demand_F = demand.col(3)/4.2/15;
    VectorXd Demand_G = demand.col(4)/4.2/15;

    std::ofstream massflow("outputs/mflow.csv");
    massflow << "m1," << "m2," << "m3," << "m4," << "m5," << "m6," << "m7\n";
    for(int i{0}; i < demand.rows(); ++i) {
        const auto [m0, m1, m2, m3, m4, m5, m6] = newtonXd(Demand_C(i), Demand_D(i), Demand_E(i), Demand_F(i), Demand_G(i));
        massflow << m0 << "," << m1 << "," << m2 << "," << m3 << "," << m4 << "," << m5 << "," << m6 << "\n";
    }

}
