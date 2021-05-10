#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;

static void print(const std::vector<int> &vec)
{
    for(const auto& elem : vec){
        cerr << elem << "\t";
    }
    cerr << "\n";
}
int main()
{
    ifstream in("network1.csv");

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

    for(size_t i=0; i<data.size(); ++i)
    {
        for(int j=0; j<data[0].size(); j++)
            cerr<<data[i][j]<<" ";
        cerr<<"\n";
    }
//    const int n = stoi(data[0][0]);
//    const int m = stoi(data[0][1]);
//    std::cerr << "the number of nodes is " << n << "\n";
//    std::cerr << "the number of pipes is " << m << "\n";
  //  https://stackoverflow.com/questions/56816493/design-a-simple-c-iterator-for-matrix/56824094#56824094
 //   std::cerr << data[0][0];
//    for(auto &row : data)
//        for(auto &col : row)
//            std::cout << col << "\n";


  //  https://stackoverflow.com/questions/36260209/converting-string-array-to-int-array-in-c-cli

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

    for(auto c5 : col5){
        col5_int.push_back(stoi(c5));
    }

    std::cerr << "column 4 is " << "\n";
//    for(auto c4: col4_int){
//        std::cerr << c4 << "\t";
//    }
//    std::cerr << "\n";
    print(col4_int);
    std::cerr << "column 5 is " << "\n";
//    for(auto c5: col5_int){
//        std::cerr << c5 << "\t";
//    }
    print(col5_int);



    return 0;
}

//#include "read_data.h"
//int main()
//{
//    std::vector<std::string> vecOfStr;
//    // Get the contents of file in a vector
//    bool result = getFileContent("network1.csv", vecOfStr);
//    if(result)
//    {
//        // Print the vector contents
//        for(std::string & s : vecOfStr)
//            std::cout<<s<<std::endl;
//    }
//}