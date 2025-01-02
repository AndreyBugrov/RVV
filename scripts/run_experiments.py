import csv # necessary
import subprocess # necessary
import argparse # necessary
import shlex # necessary
import sys # necessary
import logging # necessary
from pathlib import Path # necessary
from copy import copy

from create_plots import save_no_vec_plots_and_init_ax, save_vec_plots


LOGGER = logging.getLogger(__name__)


OPERATIONS = {'vector' : 'vec_p', 'matrix' : 'mat_p', 'gram_schmidt' : 'gs_p', 'qr': 'qr_d'}
OPTIMIZATIONS    = {'simple': 'sim', 'std' : 'std', 'scalar': 'sca', 'hl_opt' : 'hlo', 'intrinsic': 'int', 'll_opt': 'llo'} # scalar means based on optimal scalar product, hl_opt - hi-level optimized


def critical_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)


BASE_OPTIMIZATION_LEVEL = "-O2"


COMPILATION_PROFILE_TO_OPTIONS = {
    "debug": "-O0",
    "release": "-O2",
    "O3": "-O3",
    "fast": "-Ofast",
    "native": f"{BASE_OPTIMIZATION_LEVEL} -march=native",
    "math": f"{BASE_OPTIMIZATION_LEVEL} -ffast-math",
    "lto": f"{BASE_OPTIMIZATION_LEVEL} -flto -fuse-linker-plugin",
    "optimal": f"{BASE_OPTIMIZATION_LEVEL} -flto -fuse-linker-plugin -march=native -ffast-math"
}


def compile_source(source_file_list: list[str], bin_path: str, optimization_profile: str):
    args = 'g++ -Wall -Werror -Wsign-compare -std=c++20 ' + COMPILATION_PROFILE_TO_OPTIONS[optimization_profile] + ' ' + ' '.join(source_file_list) + ' -o ' + bin_path
    cmd = shlex.split(args)
    LOGGER.debug("Compilation arguments: " + " ".join(cmd))
    compiler_errors = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[1]
    if compiler_errors:
        critical_message('Compilation errors:\n'+compiler_errors.decode('utf-8'))


def create_cache_list_file(file_path: str):
    args = 'getconf -a | grep CACHE'
    cache_info = subprocess.Popen(args, stdout=subprocess.PIPE, shell=True).communicate()[0].decode('utf-8').split('\n')[:-1]
    with open(file_path, 'w') as f:
        for line in cache_info:
            cache_level_info = line.split()
            if(len(cache_level_info)>1):
                f.write(f'{cache_level_info[1]}\n')
            else:
                f.write('0\n')


def get_available_cores():
    with open('/proc/cpuinfo', 'r') as f:
        return [int(line[:-1].split(': ')[1]) for line in f.readlines() if line.startswith('processor')]


def get_min_max_frequencies():
    """
    Returns:
        min and max frequencies
    """
    with open(f'/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies', 'r') as f:
        avaliable_frequencies = [int(num) for num in f.readline()[:-1].split()]
    return min(avaliable_frequencies), max(avaliable_frequencies)


def set_min_core_frequency_limit(frequency, core_num):
    line = f"sudo sh -c 'echo {frequency} > /sys/devices/system/cpu/cpu{core_num}/cpufreq/scaling_min_freq'"
    subprocess.Popen(line, shell=True).communicate()


def run_binary(bin_path: str, function_name: str, sizes: str, exp_count: int) -> list[str]:
    args = f"{bin_path} {exp_count} {function_name} {sizes} experiments.log"
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    output = proc.communicate()
    if proc.returncode:
        critical_message(f'Process has completed with non-zero return code: {proc.returncode}')
    stderr = output[1]
    if stderr:
        error_output = stderr.decode('utf-8')
        critical_message(f'Errors while running:\n{error_output}')
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
    return output[:-1]


VEC_NAMES = ["Experiments count", "Length", "Time"]
MAT_NAMES = ["Experiments count", "1st row count", "1st column count", "2nd column count", "Time"]
GS_NAMES = ["Experiments count", "Vector system size", "Vector length", "Time"]
QR_NAMES = ["Experiments count", "Row count", "Column count", "Time"]


def run_experiment(bin_path: str, function_name: str, sizes: list[int], exp_count: int, device_type: str, frequency: int, result_directory: Path):
    min_n = sizes[0]
    max_n = sizes[1] + 1
    step_n = sizes[2]

    optimization_class = function_name.split('_')[-1]
    operation_class = function_name.replace('_' + optimization_class, "")

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
        row = run_binary(bin_path=bin_path, function_name=function_name, sizes=current_sizes, exp_count=exp_count)
        with open(csv_file_name, 'a', encoding='utf-8') as f:
            writer = csv.writer(f, delimiter=';')
            writer.writerow(row)
        LOGGER.debug(f'Experiment with {current_sizes} sizes of {function_name} was carried out\n')
    LOGGER.info(f'Results were saved to {csv_file_name}')


def create_function_dict() -> dict[str, set[str]]:
    # create function dict
    function_names_dict = dict()
    # add support for operations
    function_names_dict['vector'] = {OPERATIONS['vector'] + '_' + OPTIMIZATIONS['simple'], OPERATIONS['vector']+ '_' + OPTIMIZATIONS['std']}
    function_names_dict['matrix'] = {OPERATIONS['matrix'] + '_' + OPTIMIZATIONS['simple']}
    function_names_dict['gram_schmidt'] = {OPERATIONS['gram_schmidt'] + '_' + OPTIMIZATIONS['simple']}
    function_names_dict['qr'] = {OPERATIONS['qr'] + '_' + OPTIMIZATIONS['simple']}
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


FUNCTION_NAMES_DICT = create_function_dict()


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


def create_plots(plot_format: str, result_directory: str):
    LOGGER.debug("Saving not vector plots")
    if LOGGER.level == logging.DEBUG:
        LOGGER.setLevel(logging.INFO)
        save_no_vec_plots_and_init_ax(plot_format=plot_format, time_name=VEC_NAMES[-1], result_directory=result_directory, mat_names=MAT_NAMES, gs_names=GS_NAMES, qr_names=QR_NAMES)
        LOGGER.setLevel(logging.DEBUG)
    else:
        save_no_vec_plots_and_init_ax(plot_format=plot_format, time_name=VEC_NAMES[-1], result_directory=result_directory, mat_names=MAT_NAMES, gs_names=GS_NAMES, qr_names=QR_NAMES)
    LOGGER.debug("Saving vector only plots")
    if LOGGER.level == logging.DEBUG:
        LOGGER.setLevel(logging.INFO)
        save_vec_plots(plot_format=plot_format, time_name=VEC_NAMES[-1], result_directory=result_directory, vec_names=VEC_NAMES)
        LOGGER.setLevel(logging.DEBUG)
    else:
        save_vec_plots(plot_format=plot_format, time_name=VEC_NAMES[-1], result_directory=result_directory, vec_names=VEC_NAMES)


def complete_experiment():
    print("Done!")
    exit(0)


PARENT_DIRECTORY = Path(".").parent.parent


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Matrix multiplication experiments run automatization")
    parser.add_argument('--profile', help="Type of experiment run", choices=["dry_run", "normal", "compilation", "smoke_test"], default="normal")
    parser.add_argument('-f', '--function', help="Specific functions", choices=list(FUNCTION_NAMES_DICT['all']) + ['all'], nargs='*')
    parser.add_argument('--optimization-class', help="Optimization classes", choices=list(OPTIMIZATIONS.keys()) + ['all'], nargs='*')
    parser.add_argument('--operation-class', help="Operation classes", choices=list(OPERATIONS.keys()) + ['all'], nargs='*')
    parser.add_argument('-s', "--sizes", help="Sizes that will be passed as function arguments", metavar=('MIN_SIZE', 'MAX_SIZE', 'STEP'), type=int, nargs=3, required=True)
    parser.add_argument('-O', '--opt-profile', help="Optimization profile for the compilation", choices=COMPILATION_PROFILE_TO_OPTIONS.keys(), default='release')
    parser.add_argument('-l', '--logger-level', help="Level of supported logger messages", choices=['debug', 'info', 'warning', 'error', 'critical'], default='info')
    parser.add_argument('-n', '--exp-count', help="Number of experiments with equal parameters", type=int, required=True)
    parser.add_argument('-d', '--device-name', help="Device name", choices=["kendryte", "x86"], required=True)
    parser.add_argument('--is-temporary', help="Save the results to temporary directory or not", required=True, choices=['true', 'false'])
    parser.add_argument('-r', '--recompile', help="Recompile source files", action="store_true")
    parser.add_argument('--plot-format', help="Plot format", choices=["png", "pdf", "svg"], default="png")

    args = parser.parse_args()
    profile = args.profile
    specific_functions = args.function
    operation_classes = args.operation_class
    optimization_classes = args.optimization_class
    sizes = args.sizes
    opt_profile = args.opt_profile
    logger_level = args.logger_level
    exp_count = args.exp_count
    device_name = args.device_name
    is_temporary = args.is_temporary == 'true'
    recompile = args.recompile
    plot_format = args.plot_format

    logger_level_paramenter = 0
    if logger_level == 'degug':
        logger_level_paramenter = logging.DEBUG
    elif logger_level == 'info':
        logger_level_paramenter = logging.INFO
    elif logger_level == 'warning':
        logger_level_paramenter = logging.WARNING
    elif logger_level == 'error':
        logger_level_paramenter = logging.ERROR
    elif logger_level == 'critical':
        logger_level_paramenter = logging.CRITICAL
    logging.basicConfig(level=logger_level_paramenter, format='[%(asctime)s] %(levelname)s [%(name)s:%(lineno)s] %(message)s')

    if int(exp_count) < 1:
        critical_message("Choose at least one experiment!")
    if sizes[2] <= 0:
        critical_message("Step should be more than 0!") # can be changed in next versions of "sizes" support
    if sizes[0] > sizes[1]:
        critical_message("\"min_n\" should be less or equal \"max_n\"!")

    if is_temporary:
        LOGGER.warning('Results will be saved to temporary directory')
    else:
        LOGGER.warning('Results will be saved to the ordinary directory')

    if profile == "smoke_test":
        function_names_set = get_function_name_set(["all"], [], [])
    else:
        function_names_set = get_function_name_set(specific_functions, operation_classes, optimization_classes)
    LOGGER.debug(f'Chosen functions: {function_names_set}')

    bin_directory = PARENT_DIRECTORY / 'bin'
    bin_directory.mkdir(parents=True, exist_ok=True)
    bin_path = bin_directory / (device_name + '_exp.out')

    LOGGER.info("Start of preprocessing phase")
    if recompile:
        source_file_list = []
        source_file_list.extend([str(item) for item in Path("algorithms").glob("*.cpp")])
        source_file_list.extend([str(item) for item in Path("common").glob("*.cpp")])
        source_file_list.extend([str(item) for item in Path("experiments").glob("*.cpp")])
        LOGGER.debug("Source file list: " + " ".join(source_file_list))

        LOGGER.info('Compilation')
        # recompilation does not garantees cold start (garanteed only for first function)
        compile_source(source_file_list, str(bin_path), optimization_profile=opt_profile)
    else:
        LOGGER.warning('Compilation is skipped')

    if profile == "compilation":
        complete_experiment()

    # LOGGER.info("Creating cache list file")
    # create_cache_list_file('cache.txt')

    LOGGER.info("Preparing result directory")
    result_directory = copy(PARENT_DIRECTORY)
    if is_temporary or profile == "smoke_test":
        result_directory = result_directory / "staging_results"
    else:
        result_directory = result_directory / "prod_results"
    result_directory.mkdir(parents=True, exist_ok=True)

    if profile == "dry_run":
        LOGGER.info("Experiment execution")
        for function_item in function_names_set:
            LOGGER.info(f'Process \"{function_item}\" function')
        create_plots(plot_format=plot_format, result_directory=str(result_directory))
        complete_experiment()

    core_nums = get_available_cores()
    frequencies = get_min_max_frequencies()
    if profile == "smoke_test":
        sizes = [5, 10, 5]
        exp_count = 2

    try:
        LOGGER.info('Frequency setting')
        for core in core_nums:  
            set_min_core_frequency_limit(frequencies[1], core)
        LOGGER.info("Experiment execution")
        for function_item in function_names_set:
            LOGGER.info(f'Process \"{function_item}\" function')
            run_experiment(bin_path, function_item, sizes, exp_count, device_name, frequencies[1], result_directory)
    except KeyboardInterrupt:
        critical_message('Program has been interrupted')
    finally:
        for core in core_nums:
            set_min_core_frequency_limit(frequencies[0], core)
    LOGGER.info("Plotting graphs")
    create_plots(plot_format=plot_format, result_directory=str(result_directory))
    complete_experiment()