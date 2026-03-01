import logging # necessary
import shlex # necessary
import subprocess # necessary
from typing import Generator

from pathlib import Path

from common_defs import abort_with_message, PARENT_DIRECTORY, X86_NAME, RISC_V_NAME

LOGGER = logging.getLogger(__name__)

COMPILATION_PROFILES = ["debug", "release", "O3", "fast", "base", "math", "lto", "optimal"]
DEVICE_OPTIMIZATIONS = {
    X86_NAME: "-march=native",
    RISC_V_NAME: "-march=rv64imafdcv_zicbom_zicboz_zicntr_zicond_zicsr_zifencei_zihintpause_zihpm_zfh_zfhmin_zca_zcd_zba_zbb_zbc_zbs_zkt_zve32f_zve32x_zve64d_zve64f_zve64x_zvfh_zvfhmin_zvkt_sscofpmf_sstc_svinval_svnapot_svpbmt"
}
COMMON_OPTIMIZATION_OPTIONS = '-fopenmp-simd -fopenmp'

OPTIMIZATION_LEVELS = {
    COMPILATION_PROFILES[0]: f"-O0 {COMMON_OPTIMIZATION_OPTIONS}",
    COMPILATION_PROFILES[1]: f"-O2 {COMMON_OPTIMIZATION_OPTIONS}",
    COMPILATION_PROFILES[2]: f"-O3 {COMMON_OPTIMIZATION_OPTIONS}",
    COMPILATION_PROFILES[3]: f"-Ofast {COMMON_OPTIMIZATION_OPTIONS}",
}
OPTIMIZATION_LEVELS[COMPILATION_PROFILES[4]] = OPTIMIZATION_LEVELS[COMPILATION_PROFILES[1]]
BASE_OPTIMIZATION_LEVEL = OPTIMIZATION_LEVELS[COMPILATION_PROFILES[4]]
EXTRA_OPTIONS_OFFSET = 5
OPTIMIZATION_LEVELS[COMPILATION_PROFILES[4]] = OPTIMIZATION_LEVELS[COMPILATION_PROFILES[1]]
BASE_OPTIMIZATION_LEVEL = OPTIMIZATION_LEVELS[COMPILATION_PROFILES[4]]
EXTRA_OPTIONS_OFFSET = 5
EXTRA_OPTIMIZATIONS = {
    COMPILATION_PROFILES[EXTRA_OPTIONS_OFFSET]: "-ffast-math",
    COMPILATION_PROFILES[EXTRA_OPTIONS_OFFSET + 1]: "-flto=auto -fuse-linker-plugin",
}


def _get_optimization_options(compilation_profile: str, device_name: str) -> str:
    optimization_options = f"{DEVICE_OPTIMIZATIONS[device_name]}"
    if compilation_profile in OPTIMIZATION_LEVELS.keys():
        optimization_options += f" {OPTIMIZATION_LEVELS[compilation_profile]}"
    elif compilation_profile in EXTRA_OPTIMIZATIONS:
        optimization_options += f" {BASE_OPTIMIZATION_LEVEL} {EXTRA_OPTIMIZATIONS[compilation_profile]}"
    elif compilation_profile == COMPILATION_PROFILES[-1]:
        all_extra_optimizations = " " + " ".join([option for option in EXTRA_OPTIMIZATIONS.values()])
        optimization_options +=  f" {BASE_OPTIMIZATION_LEVEL} {all_extra_optimizations}"
    else:
        abort_with_message(f"Invalid compilation profile '{compilation_profile}'. Available compilation profiles: {', '.join(COMPILATION_PROFILES)}")
    return optimization_options


def _create_bin_directory() -> Path:
    bin_directory = PARENT_DIRECTORY / 'bin'
    bin_directory.mkdir(parents=True, exist_ok=True)
    return bin_directory


def _get_bin_path(compilation_profile: str, compilation_type: str) -> Path:
    bin_directory = _create_bin_directory()
    bin_name = f"{compilation_type}_{compilation_profile}.out"
    # bin = bin_name.replace("/", "_").replace("\\", "_").replace(":", "_").replace("*", "_").replace("?", "_").replace("<", "_").replace(">", "_").replace("|", "_").replace('"', '_').replace("'", '_').replace(" ", "_").lower()
    bin_path = bin_directory / bin_name
    return bin_path


def _get_source_files_list(is_test: bool) -> list[str]:
    source_file_list = []
    source_file_list.extend([str(item) for item in Path("algorithms").glob("*.cpp")])
    source_file_list.extend([str(item) for item in Path("common").glob("*.cpp")])
    if is_test:
        source_file_list.extend([str(item) for item in Path("tests").glob("*.cpp")])
    else:
        source_file_list.extend([str(item) for item in Path("experiments").glob("*.cpp")])
    return source_file_list


def _get_specific_options_line(compilation_profile: str, compilation_type: str, eigen_path: Path | None) -> str:
    if compilation_type == "test":
        specific_options = "-fsanitize=address,undefined -fno-sanitize-recover=all"
        if compilation_profile in ["optimal", "math", "fast"]:
            specific_options += " -fno-finite-math-only"  # for nan tests in Gram-Schmidt process
        return specific_options
    specific_options =  f"-I {eigen_path}"
    if compilation_type == "perf":
        specific_options += " -g"
    return specific_options


def _get_compilation_commands(bin_path: Path, compilation_profile: str, device_name: str, compilation_type: str, eigen_path: Path | None) -> list[str]:
    is_test = compilation_type == "test"
    source_file_list = _get_source_files_list(is_test)
    LOGGER.debug("Source file list: " + " ".join(source_file_list))

    optimization_options = _get_optimization_options(compilation_profile, device_name)

    specific_options = _get_specific_options_line(compilation_profile=compilation_profile, compilation_type=compilation_type, eigen_path=eigen_path)
    if device_name == RISC_V_NAME:
        specific_options += " -DRISCV_ARCH"
    else:
        specific_options += " -DX86_ARCH"

    args = f"ccache g++ -Wall -Werror -Wsign-compare -std=c++20 -fdiagnostics-color=always -fno-omit-frame-pointer {optimization_options} {specific_options} {' '.join(source_file_list)} -o {bin_path}"
    cmd = shlex.split(args)
    LOGGER.debug(f"Compilation command line: {args}")
    return cmd


def _compile_sources(cmd: list[str]):
    LOGGER.info('Compilation...')
    compiler_errors = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[1]
    if compiler_errors:
        abort_with_message(f"Compilation errors:\n{compiler_errors.decode('utf-8')}")
    LOGGER.info('Compilation done')


def get_binary_path(compilation_profile: str, device_name: str, compilation_type: str, eigen_path: Path | None, no_recompile: bool) -> Path:
    """
    Returns:
        Path: path to the binary execution file
    """
    LOGGER.debug(f"Compilation type: {compilation_type}")
    bin_path = _get_bin_path(compilation_profile, compilation_type)
    if no_recompile:
        if bin_path.exists():
            LOGGER.warning('Compilation is skipped')
            return bin_path
        LOGGER.warning(f'Binary {bin_path} does not exist. Compilation will be performed.')
    if (eigen_path is None or not eigen_path.is_dir()) and compilation_type != "test":  # no recompile was checked before
        abort_with_message("Eigen library path must be specified")
    # if compilation_type == "eigen":
    #     cmd = ["cmake", "-DCMAKE_BUILD_TYPE=" + compilation_profile.upper(), "-DEIGEN3_INCLUDE_DIR=" + str(eigen_path), "."]
    # else:
    #     cmd = ["cmake", "-DCMAKE_BUILD_TYPE=" + compilation_profile.upper(), "."]
    cmd = _get_compilation_commands(bin_path, compilation_profile, device_name, compilation_type, eigen_path=eigen_path)
    _compile_sources(cmd)
    return bin_path


def translate_compilation_profiles(raw_compilation_profiles: list[str]) -> Generator:
    if "perf" not in raw_compilation_profiles:
        compilation_profiles = raw_compilation_profiles
    elif "debug" in raw_compilation_profiles:
        compilation_profiles = COMPILATION_PROFILES
    else:
        compilation_profiles = [item for item in COMPILATION_PROFILES if item != "debug"]
    LOGGER.debug(f"Chosen compilation profiles: {compilation_profiles}")
    yield from compilation_profiles
