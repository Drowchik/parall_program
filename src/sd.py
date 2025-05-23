import matplotlib.pyplot as plt

# Функция загрузки данных
def load_results(filename):
    sizes = []
    times = []
    with open(filename, 'r') as f:
        next(f)
        for line in f:
            if line.strip():
                size, time = line.strip().split()
                sizes.append(int(size))
                times.append(float(time))
    return sizes, times

# Загрузка всех данных
sizes, time1 = load_results("results1.txt")
_, time2 = load_results("results2.txt")
_, time4 = load_results("results4.txt")
_, time8 = load_results("results8.txt")

# Вычисление ускорений
speedup2 = [t1 / t2 for t1, t2 in zip(time1, time2)]
speedup4 = [t1 / t4 for t1, t4 in zip(time1, time4)]
speedup8 = [t1 / t8 for t1, t8 in zip(time1, time8)]

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(sizes, speedup2, label='Speedup 2', marker='o')
plt.plot(sizes, speedup4, label='Speedup 4', marker='s')
plt.plot(sizes, speedup8, label='Speedup 8', marker='^')

plt.xlabel('Matrix Size (N x N)')
plt.ylabel('Speedup over 1')
plt.title('Speedup Comparison by Thread Count')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
