import logging # necessary
import shlex # necessary
import subprocess # necessary

from pathlib import Path

from common_defs import critical_message, PARENT_DIRECTORY

LOGGER = logging.getLogger(__name__)
BASE_OPTIMIZATION_LEVEL = "-O2"
COMPILATION_PROFILE_TO_OPTIONS = {
    "debug": "-O0",
    "release": "-O2",
    "O3": "-O3",
    "fast": "-Ofast",
    "native": f"{BASE_OPTIMIZATION_LEVEL} -march=native",
    "math": f"{BASE_OPTIMIZATION_LEVEL} -ffast-math",
    "lto": f"{BASE_OPTIMIZATION_LEVEL} -flto=auto -fuse-linker-plugin",
    "optimal": f"{BASE_OPTIMIZATION_LEVEL} -flto=auto -fuse-linker-plugin -march=native -ffast-math"
}


def _create_bin_directory() -> Path:
    bin_directory = PARENT_DIRECTORY / 'bin'
    bin_directory.mkdir(parents=True, exist_ok=True)
    return bin_directory


def _get_source_files_list(is_test: bool) -> list[str]:
    source_file_list = []
    source_file_list.extend([str(item) for item in Path("algorithms").glob("*.cpp")])
    source_file_list.extend([str(item) for item in Path("common").glob("*.cpp")])
    if is_test:
        source_file_list.extend([str(item) for item in Path("tests").glob("*.cpp")])
    else:
        source_file_list.extend([str(item) for item in Path("experiments").glob("*.cpp")])
    return source_file_list


def compile_sources(compilation_profile: str, is_test: bool, for_perf: bool) -> Path:
    """
    Returns:
        path to execution file
    """
    bin_directory = _create_bin_directory()
    if is_test:
        bin_path = bin_directory / "test.out"
    else:
        bin_path = bin_directory / "experiment.out"
    if not compilation_profile:
        LOGGER.warning('Compilation is skipped')
        return bin_path
    
    source_file_list = _get_source_files_list(is_test)
    LOGGER.debug("Source file list: " + " ".join(source_file_list))

    optimization_options = COMPILATION_PROFILE_TO_OPTIONS[compilation_profile]
    if is_test and compilation_profile in ["optimal", "math", "fast"]:
        optimization_options += " -fno-finite-math-only"  # for nan tests in Gram-Schmidt process

    if for_perf:
        perf_argument = "-fno-omit-frame-pointer"
    else:
        perf_argument = ""

    args = f"g++ -Wall -Werror -Wsign-compare -std=c++20 {perf_argument} {optimization_options} " + " ".join(source_file_list) + f" -o {bin_path}"
    cmd = shlex.split(args)
    LOGGER.debug("Compilation command line: " + " ".join(cmd))

    LOGGER.info('Compilation')
    compiler_errors = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[1]
    if compiler_errors:
        critical_message('Compilation errors:\n'+compiler_errors.decode('utf-8'))
    return bin_path


def translate_compilation_profiles(raw_compilation_profiles: list[str]) -> list[str]:
    if "perf" in raw_compilation_profiles:
        compilation_profiles = [key for key in list(COMPILATION_PROFILE_TO_OPTIONS.keys()) if key != "debug"]
        if "debug" in raw_compilation_profiles:
            compilation_profiles.append("debug")
        return compilation_profiles
    return raw_compilation_profiles
