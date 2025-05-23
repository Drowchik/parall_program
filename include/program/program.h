#include <vector>
#include <stdexcept>
#include <mpi.h>
#include <iostream>

namespace matrix {

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    Matrix() : rows(0), cols(0) {}
    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<int>(c, 0)) {}

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    int& operator()(size_t i, size_t j) { return data[i][j]; }
    const int& operator()(size_t i, size_t j) const { return data[i][j]; }

    int* rawData() {
        return data.empty() ? nullptr : data[0].data();
    }

    const int* rawData() const {
        return data.empty() ? nullptr : data[0].data();
    }

    void resize(size_t r, size_t c) {
        rows = r; cols = c;
        data.assign(r, std::vector<int>(c, 0));
    }

    std::vector<int> flatten() const {
        std::vector<int> flat(rows * cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                flat[i * cols + j] = data[i][j];
        return flat;
    }

    void unflatten(const std::vector<int>& flat, size_t r, size_t c) {
        resize(r, c);
        for (size_t i = 0; i < r; ++i)
            for (size_t j = 0; j < c; ++j)
                data[i][j] = flat[i * c + j];
    }

    Matrix multiply_parallel(const Matrix& B) const {
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
    
        size_t aRows = rows, aCols = cols, bCols = B.cols;
    
        MPI_Bcast(&aRows, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&aCols, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&bCols, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    
        std::vector<int> flatB;
        if (rank == 0) flatB = B.flatten();
        else flatB.resize(aCols * bCols);
    
        MPI_Bcast(flatB.data(), flatB.size(), MPI_INT, 0, MPI_COMM_WORLD);
    
        size_t rowsPerProc = aRows / size;
        size_t extra = aRows % size;
        size_t localRows = rowsPerProc + (rank < extra ? 1 : 0);
        size_t offset = rank * rowsPerProc + std::min((size_t)rank, extra);
    
        std::vector<int> localAflat(localRows * aCols);
        if (rank == 0) {
            std::vector<int> flatA = flatten();
            std::vector<int> sendcounts(size), displs(size);
            for (int i = 0; i < size; ++i) {
                size_t r = rowsPerProc + (i < extra ? 1 : 0);
                sendcounts[i] = r * aCols;
                displs[i] = (i * rowsPerProc + std::min(i, (int)extra)) * aCols;
            }
            MPI_Scatterv(flatA.data(), sendcounts.data(), displs.data(), MPI_INT,
                         localAflat.data(), localAflat.size(), MPI_INT, 0, MPI_COMM_WORLD);
        } else {
            MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT,
                         localAflat.data(), localAflat.size(), MPI_INT, 0, MPI_COMM_WORLD);
        }
    
        Matrix localA;
        localA.unflatten(localAflat, localRows, aCols);
        Matrix fullB;
        fullB.unflatten(flatB, aCols, bCols);
    
        Matrix localC(localRows, bCols);
        for (size_t i = 0; i < localRows; ++i)
            for (size_t j = 0; j < bCols; ++j)
                for (size_t k = 0; k < aCols; ++k)
                    localC(i, j) += localA(i, k) * fullB(k, j);
    
        std::vector<int> localCflat = localC.flatten();
        std::vector<int> recvCounts(size), displs(size);
        if (rank == 0) {
            for (int i = 0; i < size; ++i) {
                size_t r = rowsPerProc + (i < extra ? 1 : 0);
                recvCounts[i] = r * bCols;
                displs[i] = (i * rowsPerProc + std::min(i, (int)extra)) * bCols;
            }
        }
    
        std::vector<int> resultFlat;
        if (rank == 0) resultFlat.resize(aRows * bCols);
    
        MPI_Gatherv(localCflat.data(), localCflat.size(), MPI_INT,
                    resultFlat.data(), recvCounts.data(), displs.data(), MPI_INT,
                    0, MPI_COMM_WORLD);
    
        Matrix result;
        if (rank == 0)
            result.unflatten(resultFlat, aRows, bCols);
    
        return result;
    }
    
};

}
