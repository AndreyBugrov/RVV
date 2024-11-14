import csv # necessary
import subprocess # necessary
import argparse # necessary
import shlex # necessary
import sys # necessary
import logging # necessary

from pathlib import Path
from copy import copy
from itertools import zip_longest
import glob

logger = logging.getLogger(__name__)

function_classes = ['matrix', 'vector', 'gram_schmidt', 'qr', 'all']
optimization_levels = ['simple', 'std', 'scalar_based', 'hl_opt', 'intrinsic', 'll_opt'] # scalar-based means based on optimal scalar product

def create_function_dict() -> dict[str, set[str]]:
    vector_function_set = {'vec_simple', 'vec_std'}
    matrix_function_set = {'matr_simple'}
    gram_schmidt_function_set = {'gram_schmidt_simple'}
    qr_function_set = {'qr_simple'}
    all_function_set = vector_function_set + matrix_function_set + gram_schmidt_function_set + qr_function_set

    simple_function_set = {item for item in all_function_set if "simple" in item}
    scalar_based_function_set = {item for item in all_function_set if "scalar_based" in item}
    intrinsic_function_set = {item for item in all_function_set if "intrinsic" in item}
    high_level_optimal_function_set = {item for item in all_function_set if "hl_opt" in item}
    low_level_optimal_function_set = {item for item in all_function_set if "ll_opt" in item}

    function_names_dict = {'all': all_function_set, 'vector': vector_function_set, 'matrix': matrix_function_set, 
                           'gram_schmidt' : gram_schmidt_function_set, 'qr': qr_function_set,
                           'simple': simple_function_set, 'scalar_based': scalar_based_function_set, 
                           'intrinsic': intrinsic_function_set, 'hl_opt': high_level_optimal_function_set,
                           'll_opt': low_level_optimal_function_set}
    for item in all_function_set:
        function_names_dict[item] = {item}
    return function_names_dict


function_names_dict = create_function_dict()


def critical_message(msg: str):
    logger.critical(f"{msg}")
    sys.exit(-1)


def compile_source(source_file_list: list[str], bin_path: str, optimization_flag: str):
    args = 'g++ ' + ' -Wall -Werror -Wsign-compare -std=c++20 ' + optimization_flag + ' ' + ' '.join(source_file_list) + ' -o ' + bin_path
    cmd = shlex.split(args)
    logger.debug("Compilation arguments: " + " ".join(cmd))
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


def run_experiment(bin_path: str, function_name: str, sizes: str, exp_num: int) -> list[str]:
    args = f"{bin_path} {function_name} {exp_num} {sizes} experiments.log"
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    output = proc.communicate()
    stdout = output[0].decode('utf-8')  # we can't catch termination message (it is neither in stderr nor in stdout)
    stderr = output[1]
    if stderr:
        critical_message(f'Errors while running:\n{stderr.decode('utf-8')}')
    if proc.returncode:
        critical_message(f'Process has completed with non-zero return code: {proc.returncode}')
    row = stdout[:-1].split('\n')
    output = copy(sizes)
    output.extend(row)
    return output

def get_current_sizes_by_function_class(i: int, function_class: str):
    base_current_size = f'{i} '
    if function_class == 'vector':
       return base_current_size[:-1]
    elif function_class == 'matrix':
        return (base_current_size * 3)[:-1]
    elif function_class == 'gram_schmidt':
        return (base_current_size * 2)[:-1]
    elif function_class == 'qr':
        return (base_current_size * 2)[:-1]

def prepare_experiment(bin_path: str, function_class: str, optimization_level: str, sizes: list[int], exp_num: int, device_type: str, frequency: int, is_temporary: bool):
    min_n = sizes[0]
    max_n = sizes[1] + 1
    step_n = sizes[2]
    
    csv_file_name = Path(function_class + '_' + optimization_level + '_' + device_type + '_' + str(frequency / (1000*1000)) + 'GHz' + '.csv')
    if is_temporary:
        csv_file_name = Path('csv_results_tmp', csv_file_name)
    else:
        csv_file_name = Path('csv_results', csv_file_name)
    header = []
    if function_class == 'vector':
        header.extend(['Length', 'Time'])
    elif function_class == 'matrix':
        header.extend(['1st Row Count', '1st Column Count', '2nd Column Count', 'Time'])
    elif function_class == 'gram_schmidt':
        header.extend(['System Size', 'Vector Length', 'Time'])
    elif function_class == 'qr':
        header.extend(['Row Count', 'Column Count', 'Time'])
    with open(csv_file_name, 'w', encoding='utf-8') as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow(header)

    for i in range(min_n, max_n, step_n):
        current_sizes = get_current_sizes_by_function_class(i, function_class=function_class)
        row = run_experiment(bin_path=bin_path, function_name=f'{function_class}_{optimization_level}', sizes=current_sizes, exp_num=exp_num)
        with open(csv_file_name, 'a', encoding='utf-8') as f:
            writer = csv.writer(f, delimiter=';')
            writer.writerow(row)
        logger.debug(f'Experiment with {i}x{i} matrix was carried out')
    logger.info(f'Results saved to {csv_file_name}')


# there are almost 1806 pure code lines in C++ project part (#includes almost are not included, hpp has only not empty lines)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Matrix multiplication experiments run automatization", formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('-f', '--function-classes', choices=function_classes, nargs='+', help="Experiment function class", required=True)aaaaaaaaaaaaa
    parser.add_argument('-o', '--optimization-level', choices=optimization_levels, nargs='+', help="Get functions only from chosen", required=True)
    parser.add_argument('-s', "--sizes", help="Sizes that will be passed as function arguments", metavar=('MIN_SIZE', 'MAX_SIZE', 'STEP'), type=int, nargs=3, required=True)
    parser.add_argument('-l', '--opt-level', help="Optimization level in the executable file",
                        choices=['release', 'opt'], default='opt')
    parser.add_argument('-n', '--exp-num', help="Number of experiments with equal parameters", type=int, required=True)
    parser.add_argument('-d', '--device-name', help="Device name", choices=["kendryte", "x86"], required=True)
    parser.add_argument('--is-temporary', help="Should the results be saved to temporary directory or not", required=True, choices=['true', 'false'])
    parser.add_argument('-r', '--no-recompile',help='Do not recompile source files', default=True, action="store_false")
    args = parser.parse_args()
    function_class = args.function_class
    optimization_level = args.optimization_level
    sizes = args.sizes
    opt_level = args.opt_level
    exp_num = args.exp_num
    device_name = args.device_name
    is_temporary = True if args.is_temporary == 'true' else False
    recompile = args.recompile

    logging.basicConfig(level=logging.INFO, format='[%(asctime)s] %(levelname)s [%(name)s:%(lineno)s] %(message)s')
    
    if is_temporary:
        logger.warning('Results will be saved to temporary directory')
    else:
        logger.warning('Results will be saved to the base directory')
    if int(exp_num) < 1:
        critical_message("Choose at least one experiment!")
    if sizes[2] <= 0:
        critical_message("Step should be more than 0!")
    if sizes[0] > sizes[1]:
        critical_message("\"min_n\" should be less or equal \"max_n\"!")

    type_handlings = {'release': '-O2', 'opt': '-O3'}
    
    root_source_dir = Path('mat_opt')
    root_bin_dir = Path('bin')

    function_names_set = set()
    for function_item in function_names:
        function_names_set = function_names_set.union(function_names_dict[function_item])
    logger.debug(f'Chosen functions: {function_names_set}')

    bin_path = root_bin_dir / (device_name + '_exp')

    source_file_list = []
    source_file_list.extend([Path(item) for item in glob.glob("algorithms/*.cpp", root_dir="..")])
    source_file_list.extend([Path(item) for item in glob.glob("common/*.cpp", root_dir="..")])
    source_file_list.extend([Path(item) for item in glob.glob("experiments/*.cpp", root_dir="..")])

    logger.info("Start of preprocessing phase")
    if recompile:
        logger.info('Compilation')
        compile_source(source_file_list, bin_path, type_handlings[opt_level]) # recompilation does not garantees cold start (garanteed only for first function)
    else:
        logger.warning('Compilation is skipped')

    create_cache_list_file('cache.txt')

    core_nums = get_available_cores()
    frequencies = get_min_max_frequencies()
    try:
        logger.info('Frequency setting')
        for core in core_nums:
            set_min_core_frequency_limit(frequencies[1], core)

        logger.info("Start of the experiment execution phase")
        for function_item in function_names_set:
            logger.info(f'Process \"{function_item}\" function')
            run_experiment(bin_path, function_item, sizes, exp_num, device_name, frequencies[1], is_temporary)
    except KeyboardInterrupt:
        critical_message('Program has been interrupted')
    finally:
        for core in core_nums:
            set_min_core_frequency_limit(frequencies[0], core)
        print("Done!")