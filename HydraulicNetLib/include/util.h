//
// Created by leanne on 5/10/21.
//

#ifndef HYDRAULICNETWORK_UTIL_H
#define HYDRAULICNETWORK_UTIL_H
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
#endif//HYDRAULICNETWORK_UTIL_H
