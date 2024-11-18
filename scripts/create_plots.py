from decimal import Decimal, getcontext

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

getcontext().prec = 8

filepath = "prod_results/qr_d_sim_x86_2.1GHz.csv"
qr_names = ["Experiments count", "Row count", "Column count", "Time"]
df = pd.read_csv(filepath, sep=';', decimal='.', header=0, names=qr_names,
                 dtype={qr_names[0]: int, qr_names[1]: int, qr_names[2]: int, qr_names[-1]: float})

fig, ax = plt.subplots(figsize=(10, 6), dpi=100)

ax.plot(df[qr_names[1]], df["Time"], color="blueviolet", label="Базовая версия")
# ax.plot(df['n'], df['time2'], color='red', label='биномиальная куча')
# ax.plot(df['n'], df['time3'], color='purple', label='фибоначчиева куча')

ax.set_title('Время работы простейшей версии QR-разложения', fontsize=15)
ax.set_xlabel("Длина строки квадратной матрицы")
ax.set_ylabel("Время работы, секунды")

ax.ticklabel_format(style='sci', useMathText=True)
ax.locator_params(axis='x', min_n_ticks=10)
ax.locator_params(axis='y', min_n_ticks=10)

ax.legend(loc='lower right')
plt.show()