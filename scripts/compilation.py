import logging # necessary
import shlex # necessary
import subprocess # necessary

from pathlib import Path

from common_defs import critical_message, PARENT_DIRECTORY, X86_NAME, KENDRYTE_NAME

LOGGER = logging.getLogger(__name__)

COMPILATION_PROFILES = ["debug", "release", "O3", "fast", "math", "lto", "native", "optimal"]
COMMON_FLAGS = '-fopenmp-simd'
OPTIMIZATION_LEVELS = {
    COMPILATION_PROFILES[0]: f"-O0 {COMMON_FLAGS}",
    COMPILATION_PROFILES[1]: f"-O2 {COMMON_FLAGS}",
    COMPILATION_PROFILES[2]: f"-O3 {COMMON_FLAGS}",
    COMPILATION_PROFILES[3]: f"-Ofast {COMMON_FLAGS}",
}
BASE_OPTIMIZATION_LEVEL = OPTIMIZATION_LEVELS[COMPILATION_PROFILES[2]]
EXTRA_OPTIONS_OFFSET = 4
EXTRA_OPTIMIZATIONS = {
    COMPILATION_PROFILES[EXTRA_OPTIONS_OFFSET]: "-ffast-math",
    COMPILATION_PROFILES[EXTRA_OPTIONS_OFFSET + 1]: "-flto=auto -fuse-linker-plugin",
}
DEVICE_OPTIMIZATIONS = {
    X86_NAME: "-march=native",
    KENDRYTE_NAME: "-march=rv64imafdcv_zicbom_zicboz_zicntr_zicsr_zifencei_zihpm_zba_zbb_zbc_zbs_svpbmt"
}


def _get_compilation_options(compilation_profile: str, device_name: str):
    if compilation_profile in OPTIMIZATION_LEVELS:
        return OPTIMIZATION_LEVELS[compilation_profile]
    elif compilation_profile in EXTRA_OPTIMIZATIONS:
        return f"{BASE_OPTIMIZATION_LEVEL} {EXTRA_OPTIMIZATIONS[compilation_profile]}"
    if compilation_profile == "native":
        return f"{BASE_OPTIMIZATION_LEVEL} {DEVICE_OPTIMIZATIONS[device_name]}"
    elif compilation_profile == COMPILATION_PROFILES[-1]:
        all_extra_optimizations = " ".join([option for option in EXTRA_OPTIMIZATIONS.values()])
        return f"{BASE_OPTIMIZATION_LEVEL} {DEVICE_OPTIMIZATIONS[device_name]} {all_extra_optimizations}"


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


def compile_sources(compilation_profile: str, device_name: str, is_test: bool, for_perf: bool) -> Path:
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

    optimization_options = _get_compilation_options(compilation_profile, device_name)

    specific_options = "-g -fno-omit-frame-pointer"

    if is_test:
        specific_options += " -fsanitize=address,undefined -fno-sanitize-recover=all"
        if compilation_profile in ["optimal", "math", "fast"]:
            specific_options += " -fno-finite-math-only"  # for nan tests in Gram-Schmidt process

    args = f"g++ -Wall -Werror -Wsign-compare -std=c++20 {optimization_options} {specific_options} " + " ".join(source_file_list) + f" -o {bin_path}"
    cmd = shlex.split(args)
    LOGGER.debug("Compilation command line: " + " ".join(cmd))

    LOGGER.info('Compilation')
    compiler_errors = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[1]
    if compiler_errors:
        critical_message('Compilation errors:\n'+compiler_errors.decode('utf-8'))
    return bin_path


def translate_compilation_profiles(raw_compilation_profiles: list[str]) -> list[str]:
    if "perf" not in raw_compilation_profiles:
        compilation_profiles = raw_compilation_profiles
    elif "debug" in raw_compilation_profiles:
        compilation_profiles = COMPILATION_PROFILES
    else:
        compilation_profiles = [item for item in COMPILATION_PROFILES if item != "debug"]
    LOGGER.debug(f"Chosen compilation profiles: {compilation_profiles}")
    return compilation_profiles
    
