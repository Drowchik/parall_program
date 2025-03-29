import subprocess
import random
import os
import numpy as np


def generate_and_save_matrix(filename: str, size: int) -> None:
    with open(filename, "w") as file:
        file.write(f"{size} {size}\n")
        for _ in range(size):
            row = [str(random.randint(0, 100)) for _ in range(size)]
            file.write(" ".join(row) + "\n")


def run_matrix_multiplication(cpp_program, matrix_a_file, matrix_b_file, result_file):
    result = subprocess.run([cpp_program, matrix_a_file, matrix_b_file, result_file],
                            capture_output=True, text=True)
    return float(result.stdout.strip())


def read_matrix_from_file(filename: str) -> np.ndarray:
    with open(filename, "r") as file:
        rows, cols = map(int, file.readline().split())
        matrix = np.zeros((rows, cols))
        for i in range(rows):
            row = list(map(float, file.readline().split()))
            matrix[i] = row
    return matrix


if __name__ == "__main__":
    results = {}
    cpp_program = "out\\build\\x64-Debug\\console.exe"
    min_size = 100
    max_size = 1500
    num_measurements = 10
    for size in range(min_size, max_size + 1, 100):
        times = []
        matrix_a_file = f"test_matrix/A/matrix_a_{size}.txt"
        matrix_b_file = f"test_matrix/B/matrix_b_{size}.txt"
        result_file = f"test_matrix/Result/result_{size}.txt"
        for _ in range(num_measurements):
            generate_and_save_matrix(matrix_a_file, size)
            generate_and_save_matrix(matrix_b_file, size)
            time_taken = run_matrix_multiplication(
                cpp_program,
                matrix_a_file,
                matrix_b_file,
                result_file
            )
            times.append(time_taken)

            A = read_matrix_from_file(matrix_a_file)
            B = read_matrix_from_file(matrix_b_file)
            C_cpp = read_matrix_from_file(result_file)

            C_numpy = np.dot(A, B)
            if np.allclose(C_cpp, C_numpy, atol=1e-5):
                print(f"Results match for size {size}x{size}.")
            else:
                print(f"Discrepancy found for size {size}x{size}!")

        avg_time = sum(times) / num_measurements
        results[size] = avg_time

    output_file = "results.txt"
    with open(output_file, "w") as file:
        file.write("Matrix Size (NxN)\tAverage Time (seconds)\n")
        for size, avg_time in results.items():
            file.write(f"{size}\t{avg_time:.6f}\n")
