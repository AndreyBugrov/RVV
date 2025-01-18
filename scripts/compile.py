import logging # necessary
import shlex # necessary
import subprocess # necessary

from pathlib import Path

from common import critical_message

LOGGER = logging.getLogger(__name__)
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


def compile_source(bin_path: str, compilation_profile: str, flame_graph: bool):
    if not compilation_profile:
        LOGGER.warning('Compilation is skipped')
        return
    source_file_list = []
    source_file_list.extend([str(item) for item in Path("algorithms").glob("*.cpp")])
    source_file_list.extend([str(item) for item in Path("common").glob("*.cpp")])
    source_file_list.extend([str(item) for item in Path("experiments").glob("*.cpp")])
    LOGGER.debug("Source file list: " + " ".join(source_file_list))

    if flame_graph:
        flame_argument = "-fno-omit-frame-pointer"
    else:
        flame_argument = ""
    args = f"g++ -Wall -Werror -Wsign-compare -std=c++20 {flame_argument} {COMPILATION_PROFILE_TO_OPTIONS[compilation_profile]} " + " ".join(source_file_list) + f" -o {bin_path}"
    cmd = shlex.split(args)
    LOGGER.debug("Compilation arguments: " + " ".join(cmd))

    LOGGER.info('Compilation')
    compiler_errors = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[1]
    if compiler_errors:
        critical_message('Compilation errors:\n'+compiler_errors.decode('utf-8'))