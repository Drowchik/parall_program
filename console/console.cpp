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
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A, B;
    if (rank == 0) {
        A = readMatrixFromFile(argv[1]);
        B = readMatrixFromFile(argv[2]);
    }

    double start = MPI_Wtime();
    Matrix C = A.multiplyMPI(B);
    double end = MPI_Wtime();

    if (rank == 0) {
        writeMatrixToFile(argv[3], C);
        cout << "Elapsed time: " << (end - start) << " sec" << endl;
    }

    MPI_Finalize();
    return 0;
}