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
inline std::vector<std::vector<T>> outputMat(const std::vector<std::vector<T>> &mat)
{
    std::ofstream filestr("outputs/inci.csv");
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
inline std::vector<std::vector<T>> outputReduceMat(std::vector<std::vector<T>> &mat)
{
    std::ofstream filestr("outputs/inci_reduced.csv");
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

inline void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    matrix.conservativeResize(numRows,numCols);
}

struct matrix
{
    matrix(int r) : storage_(r*r), size_(r) {}

    int& operator()(int i, int j)       noexcept {
        // std::cerr << "i= " << i << "and j= " << j << " and size_ =" << size_ << "\n";
        return storage_[i + size_*(j)]; }
    const int&  operator()(int i, int j) const noexcept { return storage_[i + size_*(j)]; }

    int size() const noexcept { return size_; }

    void addEdge(matrix &adj, int u, int v)
    {
        adj(u, v) += 1;
        adj(v, u) += 1;
    }

    void displayMatrix(matrix const& adj)
    {
        for(/*std::size_t*/ int i = 0; i < adj.size(); i++)
        {
            for(/*std::size_t*/int j = 0; j < adj.size(); j++)
            {
                std::cerr << adj(i,j) << "  ";
            }
            std::cerr << "\n";
        }
    }

    void displayMatrixVec(vector<vector<int>> const& adj)
    {
        for(const auto& row : adj){
            for(const auto&s : row){
                std::cerr << s << " ";
            }
            std::cerr << "\n";
        }
    }

private:
    std::vector<int> storage_;
    int size_;
};



inline vector<vector<int>> convertMatrix(matrix const& adj){
    //vector<vector<int>> matrixVec(7);
    vector<vector<int>> matrixVec(adj.size(), vector<int> (adj.size(), 0));

    for(int i = 0; i < adj.size(); i++)
    {
        for(int j = 0; j < adj.size(); j++)
        {
            matrixVec[i][j] = adj(i,j);
        }
        std::cout << "\n";
    }
    return matrixVec;
}
#endif//HYDRAULICNETWORK_UTIL_H
