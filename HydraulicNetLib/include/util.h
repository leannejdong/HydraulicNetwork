//
// Created by leanne on 5/10/21.
//

#ifndef HYDRAULICNETWORK_UTIL_H
#define HYDRAULICNETWORK_UTIL_H
#include <Eigen/Dense>
#include <optional>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;
using std::cerr;
using namespace Eigen;


inline std::optional<string> LoadFile()
{
    ifstream file("inputs/network1.csv");
    if( file )
    {
        return /*{true, */string{
                std::istreambuf_iterator{ file },
                std::istreambuf_iterator<char>{}
        }/*}*/;
    }
    return {};
}
template <typename T>
inline void printVec(const std::vector<T> &vec)
{
    for(const auto& elem : vec){
        cerr << elem << "\t";
    }
    cerr << "\n";
}
template <typename T>
inline void printMat(const std::vector<std::vector<T>> &mat)
{
    for(const auto& row : mat){
        for(const auto& col : row){
            cerr << col << " ";
        }
        cerr << "\n" ;
    }
}

template <typename T>
inline void outputMat(const std::vector<std::vector<T>> &mat)
{
    std::ofstream filestr("inci.csv");
    for(const auto& row : mat){
        for(const auto& col : row){
            //cerr << col << ",";
            filestr << col << ",";
        }
        //cerr << "\n" ;
        filestr << "\n";
    }
}

template <typename T>
inline std::vector<std::vector<T>> outputReduceMat(std::vector<std::vector<T>> &mat)
{
    std::ofstream filestr("inci_reduced.csv");
//    for(const auto& row : mat){
//        for(const auto& col : row){
//            filestr << col << " ";
//        }
//        filestr << "\n";
//    }
    int rowErasePosition = 0;
    mat.erase(mat.begin() + rowErasePosition);
    //reprinting
   // filestr << "first row deleted\n";
    for(const auto& row : mat){
        for(const auto& col : row){
            //cerr << col << ",";
            filestr << col << ",";
        }
        //cerr << "\n" ;
        filestr << "\n";
    }
    return mat;
}

template <typename T>
inline MatrixXd makeEigenMatrixFromVectors(const vector<vector<T>> &matvalues)
{
    size_t n_rows = matvalues.size();
    size_t n_cols = matvalues[0].size();
    MatrixXd A(n_rows,n_cols);

    for (size_t i=0; i!=n_rows; ++i) {
        assert(matvalues[i].size() == n_cols);

        for (size_t j=0; j!=n_cols; ++j) {
            A(i,j) = matvalues[i][j];
        }
    }

    return A;
}
#endif//HYDRAULICNETWORK_UTIL_H
