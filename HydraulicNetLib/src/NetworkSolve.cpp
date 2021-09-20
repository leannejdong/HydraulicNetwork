#include "../include/NetworkSolve.h"
#include "../include/eigenData.h"
#include "../include/newton.h"
#include "../include/inci.h"
#include "../include/util.h"
#include "../include/graph.h"
#include "../include/Timer.h"
#include <range/v3/view.hpp>
#include <typeinfo>

using Eigen::VectorXd;
using Pair = std::vector<std::pair<int, int>>;
namespace HydraulicNetwork {
    void NetworkSolve() {
        //! get incidence matrix

        cerr << LoadFile("HydraulicNetwork/data/verona.csv").value_or("File [nw_verona.csv] could not be opened!") << "\n";
        ifstream in("HydraulicNetwork/data/verona.csv");
        //    if(!in){
        //        std::cerr << "Unable to open data\n";
        //        //return EXIT_FAILURE;
        //    } else{
        //        std::cerr << "\nData file opened successfully!\n";
        //    }
        vector<vector<string>> data;
        string line, word;
        while (getline(in, line)) {
            stringstream ss(line);
            vector<string> row;
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }
            data.push_back(row);
        }
        //! define the number of Nodes and Pipes
        const size_t n = stoul(data[0][0]);
        const size_t m = stoul(data[0][1]);
        std::cerr << "the number of nodes is " << n << "\n";
        std::cerr << "the number of pipes is " << m << "\n";

        //! Source nodes
        const size_t source = stoul(data[0][5]);
        std::cerr << "the source node is " << source << "\n";

        vector<string> col4, col5;
        std::cerr << "the number of rows is " << m << "\n";
        for (size_t i = 0; i < m; ++i) {
            col4.push_back(data[i][3]);
            col5.push_back(data[i][4]);
            //printf("Element %d: %d", i, data[i][0]);
        }

        //   std::cerr << "now let us convert vector of string of vector of ints\n";
        vector<size_t> col4_int, col5_int;
        for (auto c2 : col4) {
            col4_int.push_back(stoi(c2));
        }
        //! c++ index from 0, so subtract 1 from each element of our vector
        for (auto &e2 : col4_int) {
            e2 -= 1;
        }

        for (auto c3 : col5) {
            col5_int.push_back(stoi(c3));
        }

        for (auto &e5 : col5_int) {
            e5 -= 1;
        }

        std::cerr << "Measure time taken for generating incidence matrix\n";
        HydraulicNetwork::Timer t0;
        vector<vector<int>> matA = HydraulicNetwork::gen_mat(m, n, col4_int, col5_int);
        MatrixXd A_eigen = makeEigenMatrixFromVectors(matA);

        MatrixXd A_tran = -A_eigen.transpose();
        //! Loop detection
        //! First we need adjacency matrix
        Pair V;
        for (size_t i{0}; i < col4_int.size(); ++i) {
            V.emplace_back(col4_int[i], col5_int[i]);
        }
        HydraulicNetwork::graph g(150);
        for (auto &v : V) {
            g.addEdge(v.first, v.second);
        }

        //! apply gotlieb algorithm to detect loops
        std::vector<int> cycles;
        //! find cycles from the  adjacency matrix
        //! convert the matrix type adj to double vector

        std::cerr << "Measure time taken for running Gotlieb algorithm for cycle detection\n";
        HydraulicNetwork::Timer t1;
        g.Gotlieb123(std::back_inserter(cycles));
     //   std::ofstream of("cycles.data");
        std::cout << "Print Cycles "
                  << "\n";
        HydraulicNetwork::print_cycles(std::begin(cycles), std::end(cycles), std::cout);
     //   HydraulicNetwork::print_cycles(std::begin(cycles), std::end(cycles), of);
        //std::ofstream filestr("outputs/inci_tran_reduced.csv");
        removeRow(A_tran, source - 1);
        //HydraulicNetwork::saveData(filestr, A_tran);

        MatrixXd demands;
        demands = HydraulicNetwork::openData("HydraulicNetwork/data/Demand_loads.csv");
        VectorXd consumers = demands.row(0);
        removeRow(demands, 0);
      //  std::ofstream output_mass_flow("outputs/mflow_verona.csv");


        MatrixXd solution = newtonXd(demands, consumers, A_tran, n, m);

    }
}