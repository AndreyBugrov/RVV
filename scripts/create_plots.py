import glob
from pathlib import Path
import os

from matplotlib import pyplot as plt
from matplotlib.ticker import StrMethodFormatter
import numpy as np
import pandas as pd


COLOR_LIST = ["blue", "red", "green", "darkorange",  "blueviolet", "brown", "pink", "lime", "gold", "deepskyblue", "lightsalmon"]
NAMES_DICT={
    "gs_p_sim_x86_2.1GHz.csv" : "Процесс ортогонализации Грама-Шмидта (базовый)",
    "mat_p_sim_x86_2.1GHz.csv" : "Умножение матриц (базовое)",
    "qr_d_sim_x86_2.1GHz.csv" : "QR-разложение матриц (базовое)",
    "vec_p_sim_x86_2.1GHz.csv" : "Базовый алгоритм",
    "vec_p_std_x86_2.1GHz.csv" : "Inner product"
}

fig, ax = plt.subplots(figsize=(10, 8), dpi=100)


def save_no_vec_plots_and_init_ax(plot_format: str, time_name: str, result_directory: str, mat_names: list[str], gs_names: list[str], qr_names: list[str]):
    for path_item in Path(result_directory).glob("*.csv"):
        file_name = path_item.name
        current_names = []
        file_name = path_item.name
        if file_name.startswith("vec"):
            continue
        elif file_name.startswith("mat"):
            current_names = mat_names
        elif file_name.startswith("gs"):
            current_names = gs_names
        elif file_name.startswith("qr"):
            current_names = qr_names
        else:
            raise KeyError(f"Unsupported begin of the file name '{file_name}'")
        
        types_dict = {}
        for i in range(len(current_names)-1):
            types_dict[current_names[i]] = int
        types_dict[current_names[-1]] = float
        
        n_list = [int(item) for item in pd.read_csv(path_item, sep=';', decimal='.', header=0)[current_names[1]].to_list()]
        break
    else:
        return
    times_dict = {}
    for path_item in Path(result_directory).glob("*.csv"):
        current_names = []
        file_name = path_item.name
        if file_name.startswith("vec"):
            continue
        elif file_name.startswith("mat"):
            current_names = mat_names
        elif file_name.startswith("gs"):
            current_names = gs_names
        elif file_name.startswith("qr"):
            current_names = qr_names
        else:
            raise KeyError(f"Unsupported begin of the file name '{file_name}'")
        
        types_dict = {}
        for i in range(len(current_names)-1):
            types_dict[current_names[i]] = int
        types_dict[current_names[-1]] = float
        
        tmp_df = pd.read_csv(path_item, sep=';', decimal='.', header=0, names=current_names, dtype=types_dict)
        times_dict[NAMES_DICT[file_name]] = tmp_df[time_name].to_list()

        # df.plot(ax=ax, colormap=COLOR_LIST[plot_index], x=df[current_names[0]], y=df[current_names[-1]], legend=NAMES_DICT[path_item])
    df = pd.DataFrame.from_dict(times_dict)
    for plot_index, column in enumerate(df.columns):
        ax.plot(n_list, df[column], color=COLOR_LIST[plot_index], label=column)

    ax.set_title('Алгоритмы', fontsize=15)
    ax.set_xlabel("Количество элементов")
    ax.set_ylabel("Время работы, секунды")
    ax.ticklabel_format(style='sci', useMathText=True)
    ax.locator_params(axis='x', min_n_ticks=10)
    ax.locator_params(axis='y', min_n_ticks=10)
    ax.xaxis.set_major_formatter(StrMethodFormatter('{x:.0f}'))
    ax.legend(loc="upper left")
    plt.savefig(os.path.join(result_directory, "no_vec.png"), bbox_inches="tight", format=plot_format)
    plt.cla()


def save_vec_plots(plot_format: str, time_name: str, result_directory: str, vec_names: list[str]):
    if not glob.glob(os.path.join(result_directory, "vec*.csv")):
        return
    base_file = glob.glob(os.path.join(result_directory, "vec*.csv"))[0]
    vec_n_list = pd.read_csv(base_file, sep=';', decimal='.', header=0,
                             dtype={vec_names[0]: int, vec_names[1]: int, vec_names[2]: float})[vec_names[1]].to_list()
    times_dict = {}
    for path_item in Path(result_directory).glob("vec*.csv"):
        file_name = path_item.name
        
        types_dict = {}
        for i in range(len(vec_names)-1):
            types_dict[vec_names[i]] = int
        types_dict[vec_names[-1]] = float
        
        tmp_df = pd.read_csv(path_item, sep=';', decimal='.', header=0, names=vec_names, dtype=types_dict)
        times_dict[NAMES_DICT[file_name]] = tmp_df[time_name].to_list()
    df = pd.DataFrame.from_dict(times_dict)
    for plot_index, column in enumerate(df.columns):
        ax.plot(vec_n_list, df[column], color=COLOR_LIST[plot_index], label=column)

    ax.set_title("Скалярное произведение векторов", fontsize=15)
    ax.set_xlabel("Количество элементов")
    ax.set_ylabel("Время работы, секунды")
    ax.xaxis.set_major_formatter(StrMethodFormatter('{x:,.0f}'))
    ax.legend(loc='upper left')
    plt.savefig(os.path.join(result_directory, "vec.png"), bbox_inches="tight", format=plot_format)
    plt.cla()
