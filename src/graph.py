import matplotlib.pyplot as plt


if __name__ == "__main__":
    sizes = []
    times = []
    with open('results.txt', 'r') as file:
        next(file)
        for line in file:
            if line.strip():
                parts = line.split()
                sizes.append(int(parts[0]))
                times.append(float(parts[1]))

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, 'bo-', label='Время выполнения')
    plt.xlabel('Размер матрицы (NxN)')
    plt.ylabel('Время (секунды)')
    plt.title('Зависимость времени выполнения от размера матрицы')
    plt.grid(True)
    plt.legend()
    plt.show()
