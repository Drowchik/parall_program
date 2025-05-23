#include <iostream>
#include <fstream>
#include <mpi.h>
#include <vector>
#include "../include/program/program.h"

using namespace std;
using namespace matrix;

Matrix readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) throw runtime_error("Cannot open file " + filename);

    size_t rows, cols;
    file >> rows >> cols;
    Matrix mat(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            file >> mat(i, j);
    return mat;
}

void writeMatrixToFile(const string& filename, const Matrix& mat) {
    ofstream file(filename);
    file << mat.getRows() << " " << mat.getCols() << endl;
    for (size_t i = 0; i < mat.getRows(); ++i) {
        for (size_t j = 0; j < mat.getCols(); ++j)
            file << mat(i, j) << " ";
        file << endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 4) {
        if (rank == 0) std::cerr << "Usage: ./program A.txt B.txt C.txt\n";
        MPI_Finalize();
        return 1;
    }

    Matrix A, B;
    if (rank == 0) {
        A = readMatrixFromFile(argv[1]);
        B = readMatrixFromFile(argv[2]);
    }

    double start_time = MPI_Wtime();
    Matrix C = A.multiply_parallel(B);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        writeMatrixToFile(argv[3], C);
        std::cout << (end_time - start_time) << "\n";
    }

    MPI_Finalize();
    return 0;
}

