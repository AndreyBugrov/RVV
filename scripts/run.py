import argparse
import logging
from pathlib import Path

from create_plots import create_plots
from compile import compile_source, COMPILATION_PROFILE_TO_OPTIONS
from preprocessing import get_available_cores, get_min_max_frequencies, set_min_core_frequency_limit, prepare_result_directory
from experiment import run_experiment, get_function_name_set, FUNCTION_NAMES_DICT, OPERATIONS, OPTIMIZATIONS
from my_tests import run_tests
from common import critical_message


LOGGER = logging.getLogger(__name__)


def set_logger_level(logger_level):
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


PARENT_DIRECTORY = Path(".").parent.parent
TEST_FILE_NAME = "test.out"


def create_bin_directory(parent_directory: Path) -> Path:
    bin_directory = parent_directory / 'bin'
    bin_directory.mkdir(parents=True, exist_ok=True)
    return bin_directory


def full_pass(compilation_profile: str, plot_format: str, function_names_set: set, sizes: list[int], exp_count: int, device_name: str, is_temporary: bool):
    LOGGER.info("Start of preprocessing phase")
    bin_directory = create_bin_directory(PARENT_DIRECTORY)
    bin_path = compile_source(bin_directory=bin_directory, compilation_profile=compilation_profile, is_test=False, for_perf=False)
    result_directory = prepare_result_directory(parent_directory=PARENT_DIRECTORY, is_temporary=is_temporary)
    core_nums = get_available_cores()
    min_frequenciy, max_frequency = get_min_max_frequencies()
    try:
        LOGGER.info('Frequency setting')
        for core in core_nums:  
            set_min_core_frequency_limit(max_frequency, core)
        LOGGER.info("Experiment execution")
        for function_item in function_names_set:
            LOGGER.info(f'Process \"{function_item}\" function')
            run_experiment(bin_path, function_item, sizes, exp_count, device_name, max_frequency, result_directory)
    except KeyboardInterrupt:
        critical_message('Program has been interrupted')
    finally:
        for core in core_nums:
            set_min_core_frequency_limit(min_frequenciy, core)
    create_plots(plot_format=plot_format, result_directory=str(result_directory))


def compilation(args):
    set_logger_level(args.logger_level)
    bin_directory = create_bin_directory(PARENT_DIRECTORY)
    if args.for_perf:
        compile_source(bin_directory=bin_directory, compilation_profile=args.compilation_profile, is_test=False, for_perf=True)
    compile_source(bin_directory=bin_directory, compilation_profile=args.compilation_profile, is_test=False, for_perf=False)


def smoke_test(args):
    function_names_set = get_function_name_set(["all"], [], [])
    set_logger_level(args.logger_level)
    full_pass(compilation_profile=args.compilation_profile, plot_format="png", function_names_set=function_names_set, sizes=[4, 8, 4], exp_count=3, device_name=args.device_name, is_temporary=True)


def experiment(args):
    if int(args.exp_count) < 1:
        critical_message("Choose at least one experiment!")
    sizes = args.sizes
    if sizes[2] <= 0:
        critical_message("Step should be more than 0!") # can be changed in next versions of "sizes" support
    if sizes[0] > sizes[1]:
        critical_message("\"min_n\" should be less or equal \"max_n\"!")
    function_names_set = get_function_name_set(args.specific_functions, args.operation_classes, args.optimization_classes)
    set_logger_level(args.logger_level)
    LOGGER.debug(f"Chosen functions: {function_names_set}")
    is_temporary = args.is_temporary == 'true'
    if is_temporary:
        LOGGER.warning("Results will be saved to temporary directory")
    else:
        LOGGER.warning("Results will be saved to the ordinary directory")
    full_pass(args.compilation_profile, args.plot_format, function_names_set, sizes, args.exp_count, args.device_name, is_temporary)


def plotting(args):
    set_logger_level(args.logger_level)
    result_directory = prepare_result_directory(parent_directory=PARENT_DIRECTORY, is_temporary=args.is_temporary)
    create_plots(plot_format=args.plot_format, result_directory=str(result_directory))


def testing(args):
    set_logger_level(args.logger_level)
    bin_directory = create_bin_directory(PARENT_DIRECTORY)
    test_count = 39
    compilation_profiles = []
    if not args.compilation_profile:
        compilation_profiles = list(COMPILATION_PROFILE_TO_OPTIONS.keys())
    else:
        compilation_profiles.append(args.compilation_profile)
    all_test_count = test_count * len(compilation_profiles)
    all_failed_test_count = 0
    for compilation_profile in compilation_profiles:
        LOGGER.info(f"Compilation_profile: {compilation_profile}")
        bin_path = compile_source(bin_directory, compilation_profile, is_test=True, for_perf=False)
        current_failed_test_count = run_tests(bin_path)
        all_failed_test_count += current_failed_test_count
    LOGGER.info(f"All test count: {all_test_count}")
    LOGGER.info(f"Failed test count: {all_failed_test_count}")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="QR decomposition run automation")

    base_parent_parser = argparse.ArgumentParser(add_help=False)
    base_parent_parser.add_argument('-d', '--device-name', help="Device name", choices=["kendryte", "x86"], required=True)
    base_parent_parser.add_argument('-l', '--logger-level', help="Level of supported logger messages", choices=['debug', 'info', 'warning', 'error', 'critical'], default='info')
    
    parent_compilation_parser = argparse.ArgumentParser(add_help=False)
    parent_compilation_parser.add_argument('-c', '--compilation-profile', help="Compilation profile (do not specify if compilation is not necessary)", choices=COMPILATION_PROFILE_TO_OPTIONS.keys())
    parent_plotting_parser = argparse.ArgumentParser(add_help=False)
    parent_plotting_parser.add_argument('--plot-format', help="Plot format", choices=["png", "pdf", "svg"], default="png")
    parent_plotting_parser.add_argument('--is-temporary', help="Save the results to temporary directory or not", choices=['true', 'false'], required=True)

    subparsers = parser.add_subparsers()
    compilation_parser = subparsers.add_parser("compilation", parents=[base_parent_parser, parent_compilation_parser], help="Sourse files compilation")
    compilation_parser.add_argument("--for-perf", help="add options for further perf record report creation", action="store_true")
    compilation_parser.set_defaults(func=compilation)
    smoke_test_parser = subparsers.add_parser("smoke_test", parents=[base_parent_parser, parent_compilation_parser], help="Small experiment validation")
    smoke_test_parser.set_defaults(func=smoke_test)
    plotting_parser = subparsers.add_parser("plot", parents=[base_parent_parser, parent_plotting_parser], help="Result directory plotting")
    plotting_parser.set_defaults(func=plotting)
    
    experiment_parser = subparsers.add_parser("experiment", parents=[base_parent_parser, parent_compilation_parser, parent_plotting_parser], help="Full experiment")
    experiment_parser.set_defaults(func=experiment)

    experiment_parser.add_argument('-f', '--function', help="Specific functions", choices=list(FUNCTION_NAMES_DICT['all']) + ['all'], nargs='*')
    experiment_parser.add_argument('--optimization-class', help="Optimization classes", choices=list(OPTIMIZATIONS.keys()) + ['all'], nargs='*')
    experiment_parser.add_argument('--operation-class', help="Operation classes", choices=list(OPERATIONS.keys()) + ['all'], nargs='*')

    experiment_parser.add_argument('-s', "--sizes", help="Sizes that will be passed as function arguments", metavar=("MIN_SIZE MAX_SIZE STEP"), type=int, nargs=3, required=True)
    experiment_parser.add_argument('-n', '--exp-count', help="Number of experiments with equal parameters", type=int, required=True)

    testing_parser = subparsers.add_parser("test", parents=[base_parent_parser, parent_compilation_parser], help="Tests for all compilation option sets")
    testing_parser.set_defaults(func=testing)

    args = parser.parse_args()
    if hasattr(args, "func"):
        args.func(args)
    print("Done!")
    exit(0)