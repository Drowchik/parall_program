#include <iostream>
#include <fstream>
#include <chrono>
#include "program/program.h"

using namespace std;
using namespace matrix;

Matrix readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filename);
    }

    size_t rows, cols;
    file >> rows >> cols;

    Matrix matrix(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file >> matrix(i, j);
        }
    }

    file.close();
    return matrix;
}

void writeMatrixToFile(const string& filename, const Matrix& matrix) {
    ofstream file(filename);
    file << matrix.getRows() << " " << matrix.getCols() << endl;
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            file << matrix(i, j) << " ";
        }
        file << endl;
    }
    file.close();
}


int main(int argc, char* argv[]) {
    Matrix A = readMatrixFromFile(argv[1]);
    Matrix B = readMatrixFromFile(argv[2]);
    auto start = chrono::high_resolution_clock::now();

    Matrix C = A * B;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    writeMatrixToFile(argv[3], C);
    cout << duration.count() << endl;
    return 0;
}