//
// Created by leanne on 5/25/21.
//

#ifndef HYDRAULICNETWORK_EIGENDATA_H
#define HYDRAULICNETWORK_EIGENDATA_H
#include<Eigen/Dense>
#include <fstream>
#include <vector>
#include <iostream>
using std::vector;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::IOFormat;
using Eigen::FullPrecision;
using std::vector;
using std::pair;
using Eigen::Matrix;
using Eigen::Map;
using Eigen::Dynamic;
using Eigen::RowMajor;


inline void saveData(std::ofstream &file, /*std::string fileName,*/ MatrixXd  matrix)
{
    //https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
    const static IOFormat CSVFormat(FullPrecision, Eigen::DontAlignCols, ", ", "\n");

   // std::ofstream file(fileName);
    if (!file) return;
    {
        file << matrix.format(CSVFormat) <<"\n";
    }
}

inline MatrixXd openData(std::string fileToOpen)
{
    vector<double> matrixEntries;

    // in this object we store the data from the matrix
    std::ifstream matrixDataFile(fileToOpen);
    if(!matrixDataFile) {
        std::cerr << "Failed to open " << fileToOpen << "\n";
        exit(EXIT_FAILURE);
    }

    // this variable is used to store the row of the matrix that contains commas
    std::string matrixRowString;

    // this variable is used to store the matrix entry;
    std::string matrixEntry;

    // this variable is used to track the number of rows
    int matrixRowNumber = 0;


    while (getline(matrixDataFile, matrixRowString)) // here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
    {
        std::stringstream matrixRowStringStream(matrixRowString); //convert matrixRowString that is a string to a stream variable.

        while (getline(matrixRowStringStream, matrixEntry, ',')) // here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
        {
            matrixEntries.push_back(stod(matrixEntry));   //here we convert the string to double and fill in the row vector storing all the matrix entries
        }
        matrixRowNumber++; //update the column numbers
    }

    // here we convert the vector variable into the matrix and return the resulting object,
    // note that matrixEntries.data() is the pointer to the first memory location at which the entries of the vector matrixEntries are stored;
    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);

}

inline VectorXd makeEigenVectorFromVectors(const vector<double> &vecvalues)
{
    size_t n = vecvalues.size();
    VectorXd b(n);

    for (size_t i = 0; i < n; ++i)
    {
        b(i) = vecvalues[i];
    }
    return b;
}

inline vector<vector<double>> makeMariceFromEigen(const MatrixXd &matvalues)
{
    size_t n_rows = matvalues.rows();
    size_t n_cols = matvalues.cols();
    vector<vector<double>> A(n_rows, vector<double> (n_cols, 0));

    for (size_t i = 0; i!= n_rows; ++i)
    {
        //assert(matvalues.col(i).size() == n_cols);

        for (size_t j = 0; j!= n_cols; ++j)
        {
            A[i][j] = matvalues(i, j);
        }
    }
    return A;
}

inline std::vector<std::pair<std::string, std::vector<double>>> read_csv(std::string filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<double>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile/*.is_open()*/) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    double val;

    // Read the column names
    if(myFile.good())
    {

        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<double> {}});
        }
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while(ss >> val){

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file, for windows user
    // myFile.close();

    return result;
}

void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    matrix.conservativeResize(numRows,numCols);
}

void removeColumn(Eigen::MatrixXd& matrix, unsigned int colToRemove)
{
    unsigned int numRows = matrix.rows();
    unsigned int numCols = matrix.cols()-1;

    if( colToRemove < numCols )
        matrix.block(0,colToRemove,numRows,numCols-colToRemove) = matrix.block(0,colToRemove+1,numRows,numCols-colToRemove);

    matrix.conservativeResize(numRows,numCols);
}
#endif//HYDRAULICNETWORK_EIGENDATA_H
