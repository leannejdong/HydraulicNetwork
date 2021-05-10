//
// Created by leanne on 5/9/21.
//

#ifndef HYDRAULICNETWORK_INCI_H
#define HYDRAULICNETWORK_INCI_H
#include <vector>

std::vector<std::vector<int>> gen_mat(size_t m, size_t n, std::vector<int> &in, std::vector<int> &out){
    // For declaring a 2D vector, we first define a 1D array of size equal to the number of rows m of the desired 2D vector
    std::vector<std::vector<int>>mat(m);
    for(size_t i{0}; i < n; ++i){
        mat[i].resize(n);
    }

    //std::vector<int> in, out;

    for (size_t i{0}; i < m; ++i)
        for (size_t j{0}; j < n; ++j){
            if(j == in[i]) {
                mat[i][j] = 1;
            }
            if(j == out[i]) {
                mat[i][j] = -1;
            }
        }
    return mat;
}

#endif//HYDRAULICNETWORK_INCI_H
