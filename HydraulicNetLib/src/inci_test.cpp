//
// Created by leanne on 5/10/21.
//
#include "../include/inci.h"
#include "doctest.h"
TEST_CASE("testing incidence matrix generator"){
    std::vector<std::vector<int>> expect{{1, -1, 0, 0, 0, 0, 0},
                {0, 1, -1, 0, 0, 0, 0},
                {0, 1, 0, -1, 0, 0, 0},
                {0, 0, 0, 1, 0, -1, 0},
                {0, 0, 0, 0, 0, 1, -1},
                {0, 0, 0, 0, -1, 0, 1},
                {0, 0, 0, -1, 1, 0, 0}};
    size_t m{7}; size_t n{7};
    std::vector<size_t> in{0, 1, 1, 3, 5, 6, 4};

    std::vector<size_t> out{1, 2, 3, 5, 6, 4, 3};
    std::vector<std::vector<int>> result = HydraulicNetwork::gen_mat(m, n, in, out);
    CHECK(result == expect);
  //  std::cerr << "incidence matrix has been derived correctly!\n";

}
