import subprocess
import shlex
import logging
from pathlib import Path

from common_defs import critical_message
from preprocessing import prepare_result_directory, get_available_cores, get_min_max_frequencies, set_min_core_frequency_limit
from compilation import compile_sources
from experiment import get_current_sizes_by_operation_class, OPERATIONS, OPTIMIZATIONS, FUNCTION_NAMES_DICT


LOGGER = logging.getLogger(__name__)
QR_ROW_LENGTH = 1000


def _get_perf_data(bin_path: Path, function_name: str, exp_count: int, compilation_profile: str, device_name: str, result_directory: Path) -> Path:
    sizes = get_current_sizes_by_operation_class(QR_ROW_LENGTH, OPERATIONS["qr"])
    perf_data_path = result_directory / f"{compilation_profile}_{function_name}_{device_name}_perf.data"
    args = f"perf record -F 99 -a -g -o {perf_data_path} -- {bin_path} {exp_count} {function_name} {sizes} experiments.log"
    perf_object_name = "Perf record"
    LOGGER.debug(f"{perf_object_name} line: {args}")
    cmd = shlex.split(args)
    LOGGER.info(f"{perf_object_name} launching")
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    LOGGER.debug(f"{perf_object_name} trace was saved to {perf_data_path}")
    output = proc.communicate()
    stderr = output[1]
    if stderr:
        perf_record_info = stderr.decode('utf-8')
        LOGGER.warning(f"{perf_object_name} output:\n{perf_record_info}")
    if proc.returncode:
        critical_message(f"Process has completed with non-zero return code: {proc.returncode}")
    return perf_data_path


def _get_perf_beginning(perf_data_path: Path) -> str:
    return str(perf_data_path).split("_perf.data")[0]


def _get_perf_script(perf_data_path: Path, perf_beginning: str):
    perf_script_path = Path(f"{perf_beginning}_perf.script")
    args = f"perf script -i {perf_data_path} --kallsyms=/proc/kallsyms | ../FlameGraph/stackcollapse-perf.pl > {perf_script_path}"
    perf_object_name = "Perf script"
    LOGGER.debug(f"{perf_object_name} line: {args}")
    LOGGER.info(f"{perf_object_name} launching")
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    LOGGER.debug(f"{perf_object_name} trace was saved to {perf_script_path}")
    output = proc.communicate()
    stderr = output[1]
    if stderr:
        perf_script_info = stderr.decode('utf-8')
        LOGGER.warning(f"{perf_object_name} output:\n{perf_script_info}")
    if proc.returncode:
        critical_message(f"Process has completed with non-zero return code: {proc.returncode}")
    return perf_script_path


def _create_flame_graph(perf_script_path: Path, perf_beginning: str, is_icicle: bool):
    if is_icicle:
        icicle_graph_options = "--reverse --inverted"
        perf_object_name = "Icicle graph"
        args = f"../FlameGraph/flamegraph.pl {icicle_graph_options} {perf_script_path} > {perf_beginning}_icicle_graph.svg"
    else:
        perf_object_name = "Flame graph"
        args = f"../FlameGraph/flamegraph.pl {perf_script_path} > {perf_beginning}_flame_graph.svg"
    LOGGER.debug(f"Flamegraph line: {args}")
    LOGGER.info(f"{perf_object_name} creating")
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    LOGGER.debug(f"{perf_object_name} was saved to {perf_script_path}")
    output = proc.communicate()
    stderr = output[1]
    if stderr:
        error_message = stderr.decode('utf-8')
        critical_message(f"{perf_object_name} errors:\n{error_message}")
    if proc.returncode:
        critical_message(f"Process has completed with non-zero return code: {proc.returncode}")


def _get_qr_function_names_from_optimization_class(optimization_classes: str) -> list[str]:
    function_names = []
    if "all" in optimization_classes:
        LOGGER.info("All optimization classes were chosen")
        optimization_classes = list(OPTIMIZATIONS.keys())
    for optimization_class in optimization_classes:
        function_name = OPERATIONS['qr'] + '_' + OPTIMIZATIONS[optimization_class]
        if function_name in FUNCTION_NAMES_DICT:
            function_names.append(function_name)
        else:
            LOGGER.warning(f"QR decomposition does not have \"{optimization_class}\" optimization class")
    LOGGER.debug(f"Function name list: {function_names}")
    return function_names


def _measure_performance_for_function(function_name, core_nums, min_frequenciy, max_frequency, bin_path, exp_count, compilation_profile, device_name, result_directory):
    try:
        LOGGER.info("Frequency setting")
        for core in core_nums:  
            set_min_core_frequency_limit(max_frequency, core)
        LOGGER.debug(f"Frequency is {max_frequency}")
        perf_data_path = _get_perf_data(bin_path, function_name, exp_count, compilation_profile, device_name, result_directory)
    except KeyboardInterrupt:
        for core in core_nums:
            set_min_core_frequency_limit(min_frequenciy, core)
        critical_message("Program has been interrupted")
    for core in core_nums:
        set_min_core_frequency_limit(min_frequenciy, core)
    perf_beginning = _get_perf_beginning(perf_data_path)
    perf_script_path = _get_perf_script(perf_data_path, perf_beginning)
    _create_flame_graph(perf_script_path, perf_beginning, is_icicle=False)
    _create_flame_graph(perf_script_path, perf_beginning, is_icicle=True)


def measure_performance(optimization_classes: set[str], compilation_profiles: list[str], exp_count: int, device_name: str, output_dir: str, suffix: str) -> list[str]:
    result_directory = prepare_result_directory(output_dir, suffix)
    function_names = _get_qr_function_names_from_optimization_class(optimization_classes)
    core_nums = get_available_cores()
    min_frequenciy, max_frequency = get_min_max_frequencies()
    for compilation_profile in compilation_profiles:
        LOGGER.info(f"Compilation_profile: {compilation_profile}")
        bin_path = compile_sources(compilation_profile, device_name, is_test=False, for_perf=True)
        for function_name in function_names:
            LOGGER.info(f'Process \"{function_name}\" function')
            _measure_performance_for_function(function_name, core_nums, min_frequenciy, max_frequency, bin_path, exp_count, compilation_profile, device_name, result_directory)