# -*- coding: utf-8 -*-

import pandas as pd
import matplotlib.pyplot as plt

# Чтение CSV-файла
df = pd.read_csv("results.csv")

# Функция для фильтрации и построения графика
def plot_method_order(df, method, order):
    filtered = df[(df['method'] == method) & (df['order'] == order)]
    sizes = filtered['size']
    times = filtered['time']

    label_map = {
        (1, 1): 'Insertion Sort (Ascending)',
        (1, 0): 'Insertion Sort (Descending)',
        (2, 1): 'Cocktail Sort (Ascending)',
        (2, 0): 'Cocktail Sort (Descending)',
        (3, 1): 'Quick Sort (Ascending)',
        (3, 0): 'Quick Sort (Descending)'
    }

    label = label_map.get((method, order), f'Method {method}, Order {order}')
    plt.plot(sizes, times, marker='o', label=label)

# Построение графиков для всех комбинаций методов и направлений
for method in [1, 2, 3]:
    for order in [0, 1]:
        plot_method_order(df, method, order)

# Настройки графика
plt.xlabel("Number of Elements")
plt.ylabel("Time (seconds)")
plt.title("Sorting Algorithm Performance Comparison")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.xscale('log')
plt.yscale('log')
plt.show()