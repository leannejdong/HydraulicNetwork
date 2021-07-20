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
using Eigen::Matrix;
using Eigen::Map;
using Eigen::Dynamic;
using Eigen::RowMajor;

namespace HydraulicNetwork {

    inline void saveData(std::ofstream &file, /*std::string fileName,*/ MatrixXd matrix) {
        //https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
        const static IOFormat CSVFormat(FullPrecision, Eigen::DontAlignCols, ", ", "\n");

        // std::ofstream file(fileName);
        if (!file) return;
        {
            for (size_t i{0}; i < matrix.rows(); ++i) {
                for (size_t j{0}; j < matrix.cols(); ++j) {
                    if (matrix(i, j) == -0) {
                        matrix(i, j) = 0;
                    }
                }
            }
            file << matrix.format(CSVFormat) << "\n";
        }
    }

    inline MatrixXd openData(std::string fileToOpen) {
        vector<double> matrixEntries;

        // in this object we store the data from the matrix
        std::ifstream matrixDataFile(fileToOpen);
        if (!matrixDataFile) {
            std::cerr << "Failed to open " << fileToOpen << "\n";
            exit(EXIT_FAILURE);
        }

        // this variable is used to store the row of the matrix that contains commas
        std::string matrixRowString;

        // this variable is used to store the matrix entry;
        std::string matrixEntry;

        // this variable is used to track the number of rows
        int matrixRowNumber = 0;


        while (getline(matrixDataFile, matrixRowString))// here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
        {
            std::stringstream matrixRowStringStream(matrixRowString);//convert matrixRowString that is a string to a stream variable.

            while (getline(matrixRowStringStream, matrixEntry, ','))// here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
            {
                matrixEntries.push_back(stod(matrixEntry));//here we convert the string to double and fill in the row vector storing all the matrix entries
            }
            matrixRowNumber++;//update the column numbers
        }

        // here we convert the vector variable into the matrix and return the resulting object,
        // note that matrixEntries.data() is the pointer to the first memory location at which the entries of the vector matrixEntries are stored;
        return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
    }

    inline VectorXd makeEigenVectorFromVectors(const vector<double> &vecvalues) {
        size_t n = vecvalues.size();
        VectorXd b(n);

        for (size_t i = 0; i < n; ++i) {
            b(i) = vecvalues[i];
        }

        return b;
    }

}
#endif//HYDRAULICNETWORK_EIGENDATA_H
