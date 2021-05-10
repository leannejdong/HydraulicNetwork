#include "../include/inci.h"
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

//static std::pair<bool, string> LoadFile()
//{
//    ifstream file("test.txt");
//    if( file )
//    {
//        return {true, string{
//                std::istreambuf_iterator{ file },
//                std::istreambuf_iterator<char>{}
//        }};
//    }
//    return {false, {}};
//}
static std::optional<string> LoadFile()
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
static void printVec(const std::vector<int> &vec)
{
    for(const auto& elem : vec){
        cerr << elem << "\t";
    }
    cerr << "\n";
}
static void printMat(const std::vector<std::vector<int>> &mat)
{
    for(const auto& row : mat){
        for(const auto& col : row){
            cerr << col << " ";
        }
        cerr << "\n" ;
    }
}
int main()
{
//    //const auto [exists, s] = LoadFile();
//    const auto s = LoadFile();
//   // if( /*s.empty()*/!exists)
//    if(s)
//    {
//        //cerr << "File [test.txt] could not be opened!\n";
//        cerr << *s << "\n";
//    }else{
//        //cerr << s << "\n";
//        cerr << "File [test.txt] could not be opened!\n";
//    }
// with std::optional, we need indeed just one line
    cerr << LoadFile().value_or("File [network1.csv] could not be opened!") << "\n";
    ifstream in("inputs/network1.csv");
//    if (!in) {
//        std::cout << "\nThe data file could not be opened!\n";
//    }
//    else {
//        std::cout << "\nData file opened successfully!\n";
//    }

    vector<vector<string> > data;
    string line, word;

    while (getline(in,line))
    {
        stringstream ss(line);
        vector<string> row;
        while (getline(ss, word, ','))
        {
            row.push_back(word);
        }
        data.push_back(row);
    }
    cerr << "The input data is parsed as " << "\n";
//    for(size_t i=0; i<data.size(); ++i)
//    {
//        for(int j=0; j<data[0].size(); j++)
//            cerr<<data[i][j]<<" ";
//        cerr<<"\n";
//    }
    for(auto &row : data){
        for(auto &col : row){
            cerr << col << " ";
        }
        cerr << "\n";
    }
  // define the number of Nodes and Pipes
    const int n = stoi(data[0][0]);
    const int m = stoi(data[0][1]);
//    std::cerr << "the number of nodes is " << n << "\n";
//    std::cerr << "the number of pipes is " << m << "\n";

    vector<string> col4, col5;
    std::cerr << "the number of rows is " << data.size() << "\n";
    for(int i = 0; i < data.size(); i++)
    {
       // std::cerr << data[i][3];
        col4.push_back(data[i][3]);
        col5.push_back(data[i][4]);
        //printf("Element %d: %d", i, data[i][0]);
    }
//    std::cerr << "column 4 is " << "\n";
//    for(auto c4: col4){
//        std::cerr << c4 << "\t";
//    }
//    std::cerr << "\n";
//    std::cerr << "column 5 is " << "\n";
//    for(auto c5: col5){
//        std::cerr << c5 << "\t";
//    }

    std::cerr << "now let us convert vector of string of vector of ints" << "\n";
    vector<int> col4_int, col5_int;
    for(auto c4 : col4){
        col4_int.push_back(stoi(c4));
    }
    // c++ index from 0, so subtract 1 from each element of our vector
    for(auto & e4 : col4_int)
    {
        e4 -= 1;
    }

    for(auto c5 : col5){
        col5_int.push_back(stoi(c5));
    }

    for(auto & e5 : col5_int)
    {
        e5 -= 1;
    }

    std::cerr << "column 4 is " << "\n";
//    for(auto c4: col4_int){
//        std::cerr << c4 << "\t";
//    }
//    std::cerr << "\n";
    printVec(col4_int);
    std::cerr << "column 5 is " << "\n";
//    for(auto c5: col5_int){
//        std::cerr << c5 << "\t";
//    }
    printVec(col5_int);
    vector<vector<int>> matA = gen_mat(m, n, col4_int, col5_int);
    printMat(matA);

    return 0;
}
