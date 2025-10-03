import argparse
import logging

from create_plots import create_plots, get_result_directories
from compilation import compile_sources, COMPILATION_PROFILES, translate_compilation_profiles
from experiment import get_function_name_set, full_experiment_pass, FUNCTION_NAMES_DICT, OPERATIONS, OPTIMIZATIONS
from testing import full_test
from performance import measure_performance
from common_defs import critical_message, get_device_name, set_logger_level


LOGGER = logging.getLogger(__name__)


def compilation(args):
    compilation_type = args.type
    is_test=False
    for_perf=False
    if compilation_type == "perf":
        for_perf = True
    elif compilation_type == "test":
        is_test = True
    bin_path = compile_sources(args.compilation_profile, args.device_name, is_test, for_perf)
    print(f"Path to binary file: {bin_path}")


def smoke_test(args):
    function_names_set = get_function_name_set(["all"], [], [])
    for compilation_profile in translate_compilation_profiles(args.compilation_profiles):
        LOGGER.info(f"Compilation profile: {compilation_profile}")
        full_experiment_pass(compilation_profile, plot_format="png", function_names_set=function_names_set,
                             sizes=[4, 8, 4], exp_count=3, device_name=args.device_name, output_dir=args.output_dir,
                             suffix=args.suffix, base_title=args.base_title, dot_title=args.dot_title)


def experiment(args):
    if int(args.exp_count) < 1:
        critical_message("Choose at least one experiment!")
    sizes = args.sizes
    if sizes[2] <= 0:
        critical_message("Step should be more than 0!") # can be changed in next versions of "sizes" support
    if sizes[0] > sizes[1]:
        critical_message("\"min_n\" should be less or equal \"max_n\"!")
    function_names_set = get_function_name_set(args.functions, args.operation_classes, args.optimization_classes)
    LOGGER.debug(f"Chosen functions: {function_names_set}")
    full_experiment_pass(args.compilation_profile, args.plot_format, function_names_set, sizes, args.exp_count,
                         args.device_name, args.output_dir, args.suffix, args.base_title, args.dot_title)


def plotting(args):
    result_directories = get_result_directories(args.output_dir, args.patterns)
    for result_directory in result_directories:
        create_plots(plot_format=args.plot_format, result_directory=result_directory, device_name=args.device_name, base_title=args.base_title, dot_title=args.dot_title)


def testing(args):
    compilation_profiles = translate_compilation_profiles(args.compilation_profiles)
    full_test(compilation_profiles, args.device_name)


def perf_measurements(args):
    exp_count = args.exp_count
    if int(exp_count) < 1:
        critical_message("Choose at least one experiment!")
    compilation_profiles = translate_compilation_profiles(args.compilation_profiles)
    measure_performance(args.optimization_classes, compilation_profiles, exp_count, args.device_name, args.output_dir, args.suffix)


def disasm(args):
    bin_path = 1
    



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="QR decomposition run automation")

    base_parent_parser = argparse.ArgumentParser(add_help=False)
    base_parent_parser.add_argument('-l', '--logger-level', help="Level of supported logger messages", choices=['debug', 'info', 'warning', 'error', 'critical'], default='info')
    base_parent_parser.set_defaults(device_name="unknown")
    
    parent_compilation_parser = argparse.ArgumentParser(add_help=False)
    parent_compilation_parser.add_argument('-c', '--compilation-profile', help="Compilation profile", choices=COMPILATION_PROFILES, required=True)
    parent_compilation_parser.add_argument('--no-recompile', help="Do not recompile sources", action="store_true")
    parent_plotting_parser = argparse.ArgumentParser(add_help=False)
    parent_plotting_parser.add_argument('--plot-format', help="Plot format", choices=["png", "pdf", "svg"], default="png")
    parent_plotting_parser.add_argument('--dot-title', help="Dot product plot title", default="Скалярное произведение векторов")
    parent_plotting_parser.add_argument('--base-title', help="Base algorithms plot title", default="Оптимизации QR-разложения")
    parent_optimization_parser = argparse.ArgumentParser(add_help=False)
    parent_optimization_parser.add_argument('-n', '--exp-count', help="Number of experiments with equal parameters", type=int, required=True)
    parent_optimization_parser.add_argument('--optimization-classes', help="Optimization classes", choices=list(OPTIMIZATIONS.keys()) + ['all'], nargs='*')
    parent_multicompilation_parser = argparse.ArgumentParser(add_help=False)
    parent_multicompilation_parser.add_argument('-c', '--compilation-profiles', help="Compilation profiles. \"perf\" means all profiles except \"debug\"", choices=COMPILATION_PROFILES + ["perf"], nargs="+", required=True)
    parent_result_parser = argparse.ArgumentParser(add_help=False)
    parent_result_parser.add_argument('-o', "--output-dir", help="Path to parent directory for result directory", required=True)
    parent_suffix_parser = argparse.ArgumentParser(add_help=False)
    parent_suffix_parser.add_argument("--suffix", help="Custom directory name part after current date", required=False)

    subparsers = parser.add_subparsers()
    compilation_parser = subparsers.add_parser("compilation", parents=[base_parent_parser, parent_compilation_parser], help="Sourse files compilation")
    compilation_parser.add_argument("--type", help="Compilation_target", choices=["experiment", "perf", "test"], required=True)
    compilation_parser.set_defaults(func=compilation)
    smoke_test_parser = subparsers.add_parser("smoke-test", parents=[base_parent_parser, parent_multicompilation_parser, parent_result_parser, parent_suffix_parser], help="Small experiment validation")
    smoke_test_parser.set_defaults(func=smoke_test)
    plotting_parser = subparsers.add_parser("plot", parents=[base_parent_parser, parent_plotting_parser, parent_result_parser], help="Result directory plotting")
    plotting_parser.add_argument('-p', '--patterns', help="Directory name part patterns", required=True, nargs="+")
    plotting_parser.set_defaults(func=plotting)
    
    experiment_parser = subparsers.add_parser("experiment", parents=[base_parent_parser, parent_compilation_parser, parent_plotting_parser, parent_result_parser, parent_suffix_parser, parent_optimization_parser], help="Full experiment run")
    experiment_parser.add_argument('--operation-classes', help="Operation classes", choices=list(OPERATIONS.keys()) + ['all'], nargs='*')
    experiment_parser.add_argument('-f', '--functions', help="Specific functions", choices=list(FUNCTION_NAMES_DICT['all']) + ['all'], nargs='*')   
    experiment_parser.add_argument('-s', "--sizes", help="Sizes that will be passed as function arguments", metavar=("MIN_SIZE", "MAX_SIZE", "STEP"), type=int, nargs=3, required=True)
    experiment_parser.set_defaults(func=experiment)

    testing_parser = subparsers.add_parser("test", parents=[base_parent_parser, parent_multicompilation_parser], help="Tests for all compilation option sets")
    testing_parser.set_defaults(func=testing)

    performance_parser = subparsers.add_parser("perf", parents=[base_parent_parser, parent_multicompilation_parser, parent_optimization_parser, parent_result_parser, parent_suffix_parser], help="Performance measurements using Linux Perf")
    performance_parser.set_defaults(func=perf_measurements)

    disasm_parser = subparsers.add_parser("disasm", parents=[base_parent_parser, parent_multicompilation_parser], help="Disassembling using Linux Perf")
    disasm_parser.set_defaults(func=disasm)

    args = parser.parse_args()
    set_logger_level(args.logger_level)
    args.device_name = get_device_name()
    if hasattr(args, "func"):
        args.func(args)
    else:
        critical_message("Parser has no function to run")
    print("Done!")
    exit(0)