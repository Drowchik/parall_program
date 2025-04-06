import matplotlib.pyplot as plt


def read_data(filename):
    sizes = []
    times = []
    with open(filename, 'r') as file:
        next(file)
        for line in file:
            if line.strip():
                parts = line.split()
                sizes.append(int(parts[0]))
                times.append(float(parts[1]))
    return sizes, times


if __name__ == "__main__":
    sizes_single, times_single = read_data('single_results.txt')

    sizes_omp, times_omp = read_data('omp_results10.txt')

    speedup = [t_single / t_omp for t_single,
               t_omp in zip(times_single, times_omp)]

    plt.figure(figsize=(12, 6))
    plt.plot(sizes_single, speedup, 'go-', label='Ускорение (speedup)')
    plt.axhline(y=1, color='k', linestyle='--', label='Нет ускорения')
    plt.xlabel('Размер матрицы N*N')
    plt.ylabel('Коэффициент ускорения')
    plt.title('Ускорение OMP (10 потока) относительно однопоточного выполнения')
    plt.grid(True)
    plt.legend()
    plt.show()
