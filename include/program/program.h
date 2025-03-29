#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>


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

	};
}