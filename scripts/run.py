import argparse
import logging

from create_plots import create_plots
from compile import compile_sources, COMPILATION_PROFILE_TO_OPTIONS
from preprocessing import prepare_result_directory
from experiment import get_function_name_set, full_experiment_pass, FUNCTION_NAMES_DICT, OPERATIONS, OPTIMIZATIONS
from my_tests import full_test
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
    compile_sources(args.compilation_profile, is_test, for_perf)


def smoke_test(args):
    function_names_set = get_function_name_set(["all"], [], [])
    full_experiment_pass(compilation_profile=args.compilation_profile, plot_format="png", function_names_set=function_names_set, sizes=[4, 8, 4], exp_count=3, device_name=args.device_name, is_temporary=True)


def experiment(args):
    if int(args.exp_count) < 1:
        critical_message("Choose at least one experiment!")
    sizes = args.sizes
    if sizes[2] <= 0:
        critical_message("Step should be more than 0!") # can be changed in next versions of "sizes" support
    if sizes[0] > sizes[1]:
        critical_message("\"min_n\" should be less or equal \"max_n\"!")
    function_names_set = get_function_name_set(args.specific_functions, args.operation_classes, args.optimization_classes)
    LOGGER.debug(f"Chosen functions: {function_names_set}")
    is_temporary = args.is_temporary == 'true'
    if is_temporary:
        LOGGER.warning("Results will be saved to temporary directory")
    else:
        LOGGER.warning("Results will be saved to the ordinary directory")
    full_experiment_pass(args.compilation_profile, args.plot_format, function_names_set, sizes, args.exp_count, args.device_name, is_temporary)


def plotting(args):
    result_directory = prepare_result_directory(is_temporary=args.is_temporary)
    create_plots(plot_format=args.plot_format, result_directory=str(result_directory))


def testing(args):
    compilation_profiles = []
    if not args.compilation_profile:
        compilation_profiles = list(COMPILATION_PROFILE_TO_OPTIONS.keys())
    else:
        compilation_profiles.append(args.compilation_profile)
    full_test(compilation_profiles=compilation_profiles)


def perf_measurements(args):
    exp_count = args.exp_count
    if int(exp_count) < 1:
        critical_message("Choose at least one experiment!")
    if not args.compilation_profiles:
        critical_message("Choose at least one compilation profile!")
    if "perf" in args.compilation_profiles:
        compilation_profiles = [key for key in list(COMPILATION_PROFILE_TO_OPTIONS.keys()) if key != "debug"]
    else:
        compilation_profiles = args.compilation_profiles
    measure_performance(args.optimization_classes, compilation_profiles, exp_count, args.device_name)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="QR decomposition run automation")

    base_parent_parser = argparse.ArgumentParser(add_help=False)
    base_parent_parser.add_argument('-l', '--logger-level', help="Level of supported logger messages", choices=['debug', 'info', 'warning', 'error', 'critical'], default='info')
    base_parent_parser.set_defaults(device_name="unknown")
    
    parent_compilation_parser = argparse.ArgumentParser(add_help=False)
    parent_compilation_parser.add_argument('-c', '--compilation-profile', help="Compilation profile (do not specify if compilation is not necessary)", choices=COMPILATION_PROFILE_TO_OPTIONS.keys())
    parent_plotting_parser = argparse.ArgumentParser(add_help=False)
    parent_plotting_parser.add_argument('--plot-format', help="Plot format", choices=["png", "pdf", "svg"], default="png")
    parent_plotting_parser.add_argument('--is-temporary', help="Save the results to temporary directory or not", choices=['true', 'false'], required=True)
    parent_optimization_parser = argparse.ArgumentParser(add_help=False)
    parent_optimization_parser.add_argument('-n', '--exp-count', help="Number of experiments with equal parameters", type=int, required=True)
    parent_optimization_parser.add_argument('--optimization-classes', help="Optimization classes", choices=list(OPTIMIZATIONS.keys()) + ['all'], nargs='*')
    parent_multicompilation_parser = argparse.ArgumentParser(add_help=False)
    parent_multicompilation_parser.add_argument('-c', '--compilation-profiles', help="Compilation profiles. \"perf\" means all profiles except \"debug\"", choices=list(COMPILATION_PROFILE_TO_OPTIONS.keys()) + ["perf"], nargs="+")

    subparsers = parser.add_subparsers()
    compilation_parser = subparsers.add_parser("compilation", parents=[base_parent_parser, parent_compilation_parser], help="Sourse files compilation")
    compilation_parser.add_argument("--type", help="Compilation_target", choices=["experiment", "perf", "test"], required=True)
    compilation_parser.set_defaults(func=compilation)
    smoke_test_parser = subparsers.add_parser("smoke_test", parents=[base_parent_parser, parent_compilation_parser], help="Small experiment validation")
    smoke_test_parser.set_defaults(func=smoke_test)
    plotting_parser = subparsers.add_parser("plot", parents=[base_parent_parser, parent_plotting_parser], help="Result directory plotting")
    plotting_parser.set_defaults(func=plotting)
    
    experiment_parser = subparsers.add_parser("experiment", parents=[base_parent_parser, parent_compilation_parser, parent_plotting_parser, parent_optimization_parser], help="Full experiment run")
    experiment_parser.add_argument('--operation-classes', help="Operation classes", choices=list(OPERATIONS.keys()) + ['all'], nargs='*')
    experiment_parser.add_argument('-f', '--functions', help="Specific functions", choices=list(FUNCTION_NAMES_DICT['all']) + ['all'], nargs='*')   
    experiment_parser.add_argument('-s', "--sizes", help="Sizes that will be passed as function arguments", metavar=("MIN_SIZE", "MAX_SIZE", "STEP"), type=int, nargs=3, required=True)
    experiment_parser.set_defaults(func=experiment)

    testing_parser = subparsers.add_parser("test", parents=[base_parent_parser, parent_multicompilation_parser], help="Tests for all compilation option sets")
    testing_parser.set_defaults(func=testing)

    performance_parser = subparsers.add_parser("perf", parents=[base_parent_parser, parent_multicompilation_parser, parent_optimization_parser], help="Performance measurements using Linux Perf")
    performance_parser.set_defaults(func=perf_measurements)

    args = parser.parse_args()
    set_logger_level(args.logger_level)
    args.device_name = get_device_name()
    if hasattr(args, "func"):
        args.func(args)
    else:
        critical_message("Parser has no function to run")
    print("Done!")
    exit(0)
