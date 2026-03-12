from pathlib import Path
import subprocess
import logging
import shlex

from preprocessing import prepare_result_directory, check_output_dir, print_result_directory
from compilation import get_optimization_options, get_specific_options_line
from common_defs import abort_with_message

BASE_STR = "ccache g++ -Wall -Werror -Wsign-compare -std=c++20 -fdiagnostics-color=always -fno-omit-frame-pointer -g -fopt-info-vec-loop-optimized-missed -S"

LOGGER = logging.getLogger(__name__)


def _get_source_files_list(dir: Path) -> list[Path]:
    return [item for item in dir.glob("*.cpp")]


def _make_subdir(dir: Path, subdir_name: str, exist_ok=False) -> Path:
    subdir = dir / subdir_name
    subdir.mkdir(exist_ok=exist_ok)
    return subdir


def _process_src(src_file: Path, base_dir: Path, optimization_options: list[str], specific_options: list[str],):
    src_name = src_file.stem
    source_file_dir = _make_subdir(base_dir, src_name)
    asm_file = source_file_dir / f"{src_name}.s"
    report_file = source_file_dir / "vec_missed.txt"
    args = f"{BASE_STR} {optimization_options} {specific_options} {src_file} -o {asm_file} 2> {report_file}"
    LOGGER.debug(f"Compiler report command line: {args}")
    proc = subprocess.Popen(args, shell=True)
    proc.communicate()
    if proc.returncode:
        with open(report_file) as f:
            error = f.read()
        abort_with_message(f"Process returned non-zero return code {proc.returncode}: {args}.\nError message:\n{error}")


def _process_compilation_profile(profile_dir: Path, optimization_options: list[str], specific_options: list[str], result_directory: Path):
    for dir in ["algorithms", "common", "experiments"]:
        LOGGER.info(f"Processing directory: {dir}")
        sources_folder_dir = _make_subdir(profile_dir, dir)
        for src_file in Path(dir).glob("*.cpp"):
            _process_src(src_file, sources_folder_dir, optimization_options, specific_options)


def get_compiler_report(device_name: str, output_dir: Path, suffix: str, compilation_profiles: list[str], eigen_path: Path):
    if not eigen_path:
        abort_with_message("Specify --lib parameter")
    check_output_dir(output_dir)
    result_directory = prepare_result_directory(output_dir, suffix)
    specific_options = get_specific_options_line(compilation_profile="", compilation_type="report", device_name=device_name, eigen_path=eigen_path)  # compilation profile is not needed here
    for compilation_profile in compilation_profiles:
        LOGGER.info(f"Processing compilation profile: {compilation_profile}")
        profile_dir = _make_subdir(result_directory, compilation_profile)
        optimization_options = get_optimization_options(compilation_profile, device_name)
        _process_compilation_profile(profile_dir, optimization_options, specific_options, result_directory)
    print_result_directory(result_directory)