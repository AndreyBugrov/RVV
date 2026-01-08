import csv
import subprocess
import shlex
import logging
from pathlib import Path

from common_defs import critical_message
from compilation import get_binary_path
from preprocessing import prepare_result_directory, get_available_cores, get_min_max_frequencies, set_min_core_frequency_limit
from create_plots import create_plots


LOGGER = logging.getLogger(__name__)


OPERATIONS = {'vector': 'vec_p', 'matrix': 'mat_p', 'gram_schmidt': 'gs_p', 'qr': 'qr_d'}
OPTIMIZATIONS = {'simple': 'sim', 'std': 'std', 'row': 'row_sim', 'dot': 'dot', 'simd': 'simd',
                 'hl_opt': 'hlo', 'intrinsic': 'int', 'll_opt': 'llo', 'full_row': 'row_row',
                 'unrolling': 'urol', 'double_unrolling': 'drol', 'block': 'block', 'inline': 'inl', 'matrix': 'matr',
                 'hh': 'hh'} # scalar means based on optimal scalar product, hl_opt - hi-level optimized, hh - householder


def terminate_experiment(error_msg: str):
    LOGGER.critical(f"{error_msg}")
    raise KeyboardInterrupt


def _run_binary(bin_path: Path, function_name: str, sizes: str, exp_count: int) -> list[str]:
    args = f"{bin_path} {exp_count} {function_name} {sizes} experiments.log"
    LOGGER.debug(f"Run args: {args}")
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = proc.communicate()
    stderr = output[1]
    if proc.returncode or stderr:
        error_message = "Run errors:"
        if proc.returncode:
            error_message += f'\nProcess terminated with a non-zero return code: {proc.returncode}\n'
        if stderr:
            error_output = stderr.decode('utf-8')
            error_message += f'\nErrors while running:\n{error_output}'
        terminate_experiment(error_message)
    stdout = output[0].decode('utf-8')  # we can't catch termination message (it is neither in stderr nor in stdout)
    LOGGER.debug('Program output: ' + stdout[:-1])
    row = stdout[:-1].split('\n')
    output = str(exp_count).split()
    output.extend(sizes.split())
    output.extend(row)
    return output


def get_current_sizes_by_operation_class(current_size: int, operation_class: str):
    base_current_size = f'{current_size} '
    output = ""
    if operation_class == OPERATIONS['vector']:
       output = f'{current_size*1000*1000}'
    elif operation_class == OPERATIONS['matrix']:
        output = base_current_size * 3
    elif operation_class == OPERATIONS['gram_schmidt']:
        output = base_current_size * 2
    elif operation_class == OPERATIONS['qr']:
        output = base_current_size * 2
    else:
        terminate_experiment(f"Wrong operation class name: {operation_class}")
    return output[:-1]


VEC_NAMES = ["Experiments count", "Length", "Time"]
MAT_NAMES = ["Experiments count", "1st row count", "1st column count", "2nd column count", "Time"]
GS_NAMES = ["Experiments count", "Vector system size", "Vector length", "Time"]
QR_NAMES = ["Experiments count", "Row count", "Column count", "Time"]


def run_experiment(bin_path: Path, function_name: str, sizes: list[int], exp_count: int, device_type: str, frequency: int, result_directory: Path):
    min_n = sizes[0]
    max_n = sizes[1] + 1
    step_n = sizes[2]

    operation_class = function_name.split("_")[0] + "_" + function_name.split("_")[1]

    csv_file_name = result_directory / Path(function_name + '_' + device_type + '_' + str(frequency / (1000*1000)) + 'GHz' + '.csv')

    header = []
    if operation_class == OPERATIONS['vector']:
        header.extend(VEC_NAMES)
    elif operation_class == OPERATIONS['matrix']:
        header.extend(MAT_NAMES)
    elif operation_class == OPERATIONS['gram_schmidt']:
        header.extend(GS_NAMES)
    elif operation_class == OPERATIONS['qr']:
        header.extend(QR_NAMES)
    with open(csv_file_name, 'w', encoding='utf-8') as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow(header)

    for i in range(min_n, max_n, step_n):
        current_sizes = get_current_sizes_by_operation_class(current_size=i, operation_class=operation_class)
        row = _run_binary(bin_path=bin_path, function_name=function_name, sizes=current_sizes, exp_count=exp_count)
        with open(csv_file_name, 'a', encoding='utf-8') as f:
            writer = csv.writer(f, delimiter=';')
            writer.writerow(row)
        LOGGER.debug(f'Experiment with {current_sizes} sizes of {function_name} was carried out\n')
    LOGGER.info(f'Results were saved to {csv_file_name}')


def full_experiment_pass(compilation_profile: str, plot_format: str, function_names_set: set, sizes: list[int], 
                         exp_count: int, device_name: str, output_dir: str, suffix: str, base_title: str,
                         dot_title: str, no_plotting: bool, no_recompile: bool):
    LOGGER.info("Start of preprocessing phase")
    result_directory = prepare_result_directory(output_dir, suffix)
    bin_path = get_binary_path(compilation_profile, device_name, compilation_type="experiment", eigen_path=None, no_recompile=no_recompile)
    core_nums = get_available_cores()
    min_frequenciy, max_frequency = get_min_max_frequencies()
    try:
        LOGGER.info("Frequency setting")
        for core in core_nums:  
            set_min_core_frequency_limit(max_frequency, core)
        LOGGER.debug(f"Frequency is {max_frequency}")
        LOGGER.info("Experiment execution")
        for function_item in function_names_set:
            LOGGER.info(f'Process \"{function_item}\" function')
            run_experiment(bin_path, function_item, sizes, exp_count, device_name, max_frequency, result_directory)
    except KeyboardInterrupt:
        for core in core_nums:
            set_min_core_frequency_limit(min_frequenciy, core)
        critical_message('Experiment was interrupted')
    for core in core_nums:
        set_min_core_frequency_limit(min_frequenciy, core)
    if no_plotting:
        return
    create_plots(plot_format=plot_format, result_directory=result_directory, device_name=device_name, base_title=base_title, dot_title=dot_title)


def _create_function_dict() -> dict[str, set[str]]:
    # create function dict
    function_names_dict = dict()
    # add support for operations
    function_names_dict['vector'] = {OPERATIONS['vector'] + '_' + OPTIMIZATIONS['simple'], OPERATIONS['vector']+ '_' + OPTIMIZATIONS['std']}
    function_names_dict['matrix'] = {OPERATIONS['matrix'] + '_' + OPTIMIZATIONS['simple'], OPERATIONS['matrix'] + '_' + OPTIMIZATIONS['row']}
    function_names_dict['gram_schmidt'] = {OPERATIONS['gram_schmidt'] + '_' + OPTIMIZATIONS['simple'],
                                           OPERATIONS['gram_schmidt'] + '_' + OPTIMIZATIONS['row'],
                                           OPERATIONS['gram_schmidt'] + '_' + OPTIMIZATIONS['simd'],
                                           OPERATIONS['gram_schmidt'] + '_' + OPTIMIZATIONS['unrolling']}
    function_names_dict['qr'] = {OPERATIONS['qr'] + '_' + OPTIMIZATIONS['simple'], OPERATIONS['qr'] + '_' + OPTIMIZATIONS['row'], 
                                 OPERATIONS['qr'] + '_' + OPTIMIZATIONS['full_row'], OPERATIONS['qr'] + '_' + OPTIMIZATIONS['simd'],
                                 OPERATIONS['qr'] + '_' + OPTIMIZATIONS['unrolling'], OPERATIONS['qr'] + '_' + OPTIMIZATIONS['double_unrolling'],
                                 OPERATIONS['qr'] + '_' + OPTIMIZATIONS['block'], OPERATIONS['qr'] + '_' + OPTIMIZATIONS['dot'],
                                 OPERATIONS['qr'] + '_' + OPTIMIZATIONS['inline'], OPERATIONS['qr'] + '_' + OPTIMIZATIONS['matrix'],
                                 OPERATIONS['qr'] + '_' + OPTIMIZATIONS['hh']}
    # add support for optimizations
    for key in OPTIMIZATIONS.keys():
        function_names_dict[key] = set()
        for _, function_set in function_names_dict.items():
            for function_name in function_set:
                if OPTIMIZATIONS[key] in function_name:
                    function_names_dict[key].add(function_name)
    # create all function set
    all_function_set = set()
    for _, value in function_names_dict.items():
        for item in value:
            all_function_set.add(item)
    # add support for specific name using all function set
    for function_name in all_function_set:
        function_names_dict[function_name] = {function_name}
    # add support for 'all' keyword
    function_names_dict['all'] = all_function_set
    # return result
    return function_names_dict


FUNCTION_NAMES_DICT = _create_function_dict()


def get_function_name_set(specific_functions: list[str], operation_classes: list[str], optimization_classes: list[str]) -> set:
    function_names_set = set()
    if specific_functions:
        if 'all' in specific_functions:
            return FUNCTION_NAMES_DICT['all']
        for function_name in specific_functions:
            function_names_set = function_names_set.union(FUNCTION_NAMES_DICT[function_name])
    if operation_classes:
        if 'all' in operation_classes:
            return FUNCTION_NAMES_DICT['all']
        for operation_class in operation_classes:
            function_names_set = function_names_set.union(FUNCTION_NAMES_DICT[operation_class])
    if optimization_classes:
        if 'all' in optimization_classes:
            return FUNCTION_NAMES_DICT['all']
        for optimization_class in optimization_classes:
            function_names_set = function_names_set.union(FUNCTION_NAMES_DICT[optimization_class])
    return function_names_set