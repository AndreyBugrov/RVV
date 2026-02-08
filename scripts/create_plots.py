from pathlib import Path
import logging

from common_defs import abort_with_message, import_module_by_name, TIME_NAME, REF_TIME_NAME, VEC_COLUMN_NAMES, MAT_COLUMN_NAMES, GS_COLUMN_NAMES, QR_COLUMN_NAMES

LOGGER = logging.getLogger(__name__)


COLOR_LIST = ["blue", "red", "forestgreen", "darkorange", "blueviolet", "lightpink", "darkgreen", "sienna", "lime", "gold", "dodblue", "lightsalmon", "midnightblue", "darkred", "cyan", "deeppink"]
QR_OPTIMIZATION_LIST = {
    "qr_d_sim_": "Базовый алгоритм",
    "qr_d_row_sim_": "Строчное умножение матриц",
    "qr_d_row_row_": "Матричный процесс Грама-Шмидта",
    "qr_d_simd_": "SIMD в процессе Грама-Шмидта",
    "qr_d_urol_": "Размотка цикла в процессе Грама-Шмидта",
    "qr_d_drol_": "Размотка цикла в обеих подзадачах",
    "qr_d_block_": "Блочное умножение без размотки цикла",
    "qr_d_dot_": "Блочное умножение с размоткой цикла",
    "qr_d_inl_": "Уменьшение умножений в процессе Грама-Шмидта",
    "qr_d_matr_" : "Векторно-матричные операции в процессе Грама-Шмидта",
    "qr_d_hh_sim_" : "Базовые отражения Хаусхолдера",
}
FILE_BEGINS_TO_GRAPH_NAMES={
    "gs_p_sim_" : (0, "Процесс ортогонализации Грама-Шмидта (базовый)"),
    "gs_p_row_" : (1, "Процесс ортогонализации Грама-Шмидта (матричный)"),
    "gs_p_simd_" : (2, "Процесс ортогонализации Грама-Шмидта (SIMD)"),
    "gs_p_urol_" : (3, "Процесс ортогонализации Грама-Шмидта (развёртка скалярного произведения)"),
    "mat_p_sim_" : (0, "Умножение матриц (базовое)"),
    "mat_p_row_sim_" : (1, "Умножение матриц (строчный алгоритм)"),
    "vec_p_sim_" : (0, "0. Базовый алгоритм"),
    "vec_p_std_" : (1, "1. Inner product"),
    "vec_p_simd_" : (2, "2. SIMD"),
    "vec_p_urol_" : (3, "3. Размотка цикла"),
    "qr_d_sim_" : (0, "0. Базовый алгоритм"),
    # "qr_d_row_sim_" : (1, "1. Строчное умножение матриц"),
    # "qr_d_row_row_" : (2, "2. Матричный процесс Грама-Шмидта"),
    # "qr_d_simd_" : (3, "3. SIMD в процессе Грама-Шмидта"),
    # "qr_d_urol_" : "4. Размотка цикла в процессе Грама-Шмидта",
    # "qr_d_drol_" : "5. Размотка цикла в обеих подзадачах",
    # "qr_d_block_" : "6. Блочное умножение без размотки цикла",
    # "qr_d_dot_" : "7. Блочное умножение с размоткой цикла",
    # "qr_d_inl_" : "8. Уменьшение умножений в процессе Грама-Шмидта",
    # "qr_d_matr_" : "9. Векторно-матричные операции в процессе Грама-Шмидта",
    # "qr_d_hh_sim_" : "10. Базовые отражения Хаусхолдера",
}


def _get_curve_name(file_name: str) -> str | None:
    for key in FILE_BEGINS_TO_GRAPH_NAMES.keys():
        if file_name.startswith(key):
            LOGGER.debug(f"{FILE_BEGINS_TO_GRAPH_NAMES[key]} was matched with {file_name}")
            return FILE_BEGINS_TO_GRAPH_NAMES[key]
    LOGGER.warning(f"{file_name} has no match with graph name")


def _get_size_list(result_directory: Path, mat_names: list[str], gs_names: list[str], qr_names: list[str]) -> list[int]:
    pd = import_module_by_name("pandas")
    for path_item in result_directory.glob("*.csv"):
        column_names = []
        file_name = path_item.name
        if file_name.startswith("vec"):
            continue
        elif file_name.startswith("mat"):
            column_names = mat_names
        elif file_name.startswith("gs"):
            column_names = gs_names
        elif file_name.startswith("qr"):
            column_names = qr_names
        else:
            abort_with_message(f"Unsupported begin of the file name \"{file_name}\"")
        return [int(item) for item in pd.read_csv(path_item, sep=';', decimal='.', header=0)[column_names[1]].to_list()]
    else:
        LOGGER.warning(f"There is no supported non-vector results files in \"{result_directory}\" directory")
        return []


def _plot_graph(times_dict: dict, step_list: list[int], title: str, x_label: str, y_label: str, plot_file_path: Path):
    pd = import_module_by_name("pandas")
    df = pd.DataFrame.from_dict(times_dict)

    plt = import_module_by_name("matplotlib.pyplot")
    _, ax = plt.subplots(figsize=(10, 8), dpi=100)
    for plot_index, column in enumerate(df.columns):
        ax.plot(step_list, df[column], color=COLOR_LIST[plot_index], label=column)
    ax.set_title(title, fontsize=15)
    # ax.set_xlabel("Количество элементов")
    # ax.set_ylabel("Время работы, секунды")
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.ticklabel_format(style='sci', axis='x', useMathText=True)
    ax.locator_params(axis='x', min_n_ticks=10)
    ax.locator_params(axis='y', min_n_ticks=10)
    ax.legend(loc="upper left")
    plt.savefig(plot_file_path, bbox_inches="tight", format=plot_file_path.suffix[1:])
    LOGGER.info(f"Plot was saved to {plot_file_path}")
    plt.cla()


def _add_time_series_to_times_dict(column_names: list[str], path_item: Path, file_name: str, time_name: str, ref_time_name: str, times_dict: dict, ref_times_dict: dict):
    pd = import_module_by_name("pandas")
    types_dict = {}
    for i in range(len(column_names)-2):
        types_dict[column_names[i]] = int
    types_dict[column_names[-2]] = float
    types_dict[column_names[-1]] = float
        
    current_csv_full_df = pd.read_csv(path_item, sep=';', decimal='.', header=0, names=column_names, dtype=types_dict)
    times_dict[_get_curve_name(file_name)] = current_csv_full_df[time_name].to_list()
    ref_times_dict[_get_curve_name(file_name)] = current_csv_full_df[ref_time_name].to_list()
    # print(times_dict)
    # sorted(times_dict)
    # print(list(times_dict.items()))


def _get_plots_path(result_directory: Path, device_name: str, is_vector: bool, plot_format: str) -> Path:
    if is_vector:
        return result_directory / f"vec_{device_name}.{plot_format}"
    else:
        return result_directory / f"non_vec_{device_name}.{plot_format}"


def _save_no_vec_plots_and_init_ax(plot_format: str, time_name: str, ref_time_name: str, result_directory: Path, device_name: str, base_title: str, mat_names: list[str], gs_names: list[str], qr_names: list[str]):
    size_list = _get_size_list(result_directory, mat_names, gs_names, qr_names)
    if not size_list:
        return
    times_dict = {}
    ref_times_dict = {}
    for path_item in result_directory.glob("*.csv"):
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
        _add_time_series_to_times_dict(current_names, path_item, file_name, time_name, ref_time_name, times_dict, ref_times_dict)

    times_dict = dict(sorted(times_dict.items()))
    ref_times_dict = dict(sorted(ref_times_dict.items()))
    plots_path = _get_plots_path(result_directory=result_directory, device_name=device_name, is_vector=False, plot_format=plot_format)
    _plot_graph(times_dict, ref_times_dict, size_list, result_directory, device_name, plot_format, base_title, is_vector=False)


def _save_vec_plots(plot_format: str, time_name: str, result_directory: Path, device_name: str, dot_title: str, vec_names: list[str]):
    pd = import_module_by_name("pandas")
    result_directories = [item for item in result_directory.glob("vec*.csv")]
    if not result_directories:
        LOGGER.warning(f"There is no supported vector results files in \"{result_directory}\" directory")
        return
    base_file = result_directories[0]
    dimension_size_list = pd.read_csv(base_file, sep=';', decimal='.', header=0,
                             dtype={vec_names[0]: int, vec_names[1]: int, vec_names[2]: float})[vec_names[1]].to_list()
    times_dict = {}
    for path_item in result_directory.glob("vec*.csv"):
        file_name = path_item.name
        _add_time_series_to_times_dict(vec_names, path_item, file_name, time_name, times_dict)
    _plot_graph(times_dict, dimension_size_list, result_directory, device_name, plot_format, dot_title, is_vector=True)


def create_plots(plot_format: str, result_directory: Path, device_name: str, base_title: str, 
                 dot_title: str, time_name: str=TIME_NAME, ref_time_name: str=REF_TIME_NAME,
                 vec_column_names: list=VEC_COLUMN_NAMES, mat_column_names: list=MAT_COLUMN_NAMES,
                 gs_column_names: list=GS_COLUMN_NAMES, qr_column_names: list=QR_COLUMN_NAMES):
    LOGGER.info(f"Creating plots for \"{result_directory}\"")
    LOGGER.info("Creating non-vector plots")
    _init_ax()
    _save_no_vec_plots_and_init_ax(plot_format, time_name, ref_time_name, result_directory, device_name, base_title, mat_names=mat_column_names, gs_names=gs_column_names, qr_names=qr_column_names)
    LOGGER.info("Creating vector only plots")
    _save_vec_plots(plot_format, time_name, result_directory, device_name, dot_title, vec_names=vec_column_names)


def get_result_directories(output_dir: str, patterns: list[str]) -> list[Path]:
    LOGGER.info("Looking for result directories")
    result_directories = []
    parent_directory = Path(output_dir)
    pattern_list_str = ", \"".join(patterns)
    LOGGER.debug(f"Pattern list: \"{pattern_list_str}\"")
    for pattern in patterns:
        extended_pattern = f"{pattern}"
        for directory_content_item in parent_directory.glob(pattern=extended_pattern):
            if not directory_content_item.is_dir():
                continue
            else:
                result_directories.append(directory_content_item)
    if not len(result_directories):
        LOGGER.warning("There is no directory found by patterns")
    else:
        result_directory_names = [str(item) for item in result_directories]
        LOGGER.info(f"Result directories: {result_directory_names}")
    return result_directories
