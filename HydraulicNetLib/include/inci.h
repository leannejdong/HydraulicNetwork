//
// Created by leanne on 5/9/21.
//

#ifndef HYDRAULICNETWORK_INCI_H
#define HYDRAULICNETWORK_INCI_H
#include <vector>

namespace HydraulicNetwork{
template <typename T>
std::vector<std::vector<int>> gen_mat(T m, T n, std::vector<T> &in, std::vector<T> &out){
    // For declaring a 2D vector, we first define a 1D array of size equal to the number of rows m of the desired 2D vector
    std::vector<std::vector<int>> mat(m, std::vector<int>(n, 0));
    for (T i{0}; i < m; ++i)
        for (T j{0}; j < n; ++j){
            if(j == in[i]) {
                mat[i][j] = 1;
            }
            if(j == out[i]) {
                mat[i][j] = -1;
            }
        }
    return mat;
}

}
#endif//HYDRAULICNETWORK_INCI_H
