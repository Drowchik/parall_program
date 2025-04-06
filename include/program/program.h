#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <omp.h>


using namespace std;

namespace matrix {

	class Matrix {
	private:
		vector<vector<int>> data;
		size_t rows;
		size_t cols;
	public:
		Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
			data.resize(rows, vector<int>(cols, 0.0));
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

			#pragma omp parallel num_threads(10)
			{
				#pragma omp for
				for (int i = 0; i < rows; ++i) {
					for (int j = 0; j < other.getCols(); ++j) {
						int sum = 0;
						for (size_t k = 0; k < cols; ++k) {
							sum += data[i][k] * other(k, j);
						}
						result(i, j) = sum;
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

	};
}