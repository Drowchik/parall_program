#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <mpi.h>


using namespace std;

namespace matrix {

	class Matrix {
	private:
		vector<vector<int>> data;
		size_t rows;
		size_t cols;
	public:
		Matrix() : rows(0), cols(0) {}

		Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
			data.resize(rows, vector<int>(cols, 0));
		}

		size_t getRows() const {
			return rows;
		}

		size_t getCols() const {
			return cols;
		}

		Matrix operator*(const Matrix& other) const {
			if (cols != other.getRows()) {
				throw invalid_argument("Matrix dimensions do not match for multiplication");
			}

			Matrix result(rows, other.getCols());

			for (size_t i = 0; i < rows; ++i) {
				for (size_t j = 0; j < other.getCols(); ++j) {
					for (size_t k = 0; k < cols; ++k) {
						result(i, j) += data[i][k] * other(k, j);
					}
				}
			}

			return result;
		}

		int& operator()(size_t row, size_t col) {
			if (row >= rows || col >= cols) {
				throw out_of_range("Matrix indices out of range");
			}
			return data[row][col];
		}

		const int& operator()(size_t row, size_t col) const {
			if (row >= rows || col >= cols) {
				throw out_of_range("Matrix indices out of range");
			}
			return data[row][col];
		}

		friend ostream& operator<<(ostream& os, const Matrix& matrix) {
			for (size_t i = 0; i < matrix.getRows(); ++i) {
				for (size_t j = 0; j < matrix.getCols(); ++j) {
					os << matrix(i, j) << " ";
				}
				os << endl;
			}
			return os;
		}

		int* rawData() { return data[0].data(); }
    const int* rawData() const { return data[0].data(); }

    Matrix multiplyMPI(const Matrix& B) const {
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        size_t aRows = rows, aCols = cols;
        size_t bRows = B.getRows(), bCols = B.getCols();

        MPI_Bcast(&aRows, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&aCols, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&bRows, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&bCols, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

        Matrix A = *this;

        size_t rowsPerProc = aRows / size;
        size_t remainder = aRows % size;
		size_t localRows = rowsPerProc + ((size_t)rank < remainder ? 1 : 0);
        size_t offset = rank * rowsPerProc + min((size_t)rank, remainder);

        Matrix localA(localRows, aCols);
        for (size_t i = 0; i < localRows; ++i)
            for (size_t j = 0; j < aCols; ++j)
                localA(i, j) = A(offset + i, j);

        Matrix localC(localRows, bCols);
        for (size_t i = 0; i < localRows; ++i)
            for (size_t j = 0; j < bCols; ++j)
                for (size_t k = 0; k < aCols; ++k)
                    localC(i, j) += localA(i, k) * B(k, j);

        Matrix result(aRows, bCols);
        vector<int> recvCounts(size), displs(size);
        for (int i = 0; i < size; ++i) {
			size_t rows = rowsPerProc + ((size_t)i < remainder ? 1 : 0);
            recvCounts[i] = rows * bCols;
            displs[i] = (i * rowsPerProc + min(i, (int)remainder)) * bCols;
        }

        MPI_Gatherv(localC.rawData(), localRows * bCols, MPI_INT,
                    result.rawData(), recvCounts.data(), displs.data(), MPI_INT,
                    0, MPI_COMM_WORLD);

        return result;
    }

	};
}