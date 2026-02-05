import subprocess
import logging
from pathlib import Path
from datetime import datetime

from common_defs import abort_with_message, do_not_setup_frequency

LOGGER = logging.getLogger(__name__)


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


def _set_min_core_frequency_limit(frequency: int, core_num):
    line = f"sudo sh -c 'echo {frequency} > /sys/devices/system/cpu/cpu{core_num}/cpufreq/scaling_min_freq'"
    subprocess.Popen(line, shell=True).communicate()


def setup_frequency(frequency: int | None, core_indeces: list[int] | None, device_name: str):
    if do_not_setup_frequency(device_name):
        return
    if frequency is None or core_indeces is None:
        abort_with_message("Frequency or core indeces are None. Aborting.")
    LOGGER.debug(f"Setting up frequency to {frequency} for cores{core_indeces} on {device_name}")
    for core_index in core_indeces:
        _set_min_core_frequency_limit(frequency, core_index)


def check_output_dir(output_dir: Path):
    LOGGER.info("Check output directory")
    if not output_dir.is_dir():
        abort_with_message(f"Output directory {output_dir} does not exist")


def prepare_result_directory(output_dir: Path, suffix: str | None) -> Path:
    current_datetime = datetime.today().strftime('%Y%m%d_%H%M%S')
    result_directory = (output_dir / current_datetime).resolve()
    if suffix:
        result_directory = Path(f"{result_directory}_{suffix}")
    try:
        result_directory.mkdir(parents=False, exist_ok=False)
    except FileExistsError:
        result_directory = Path(f"{result_directory}_1")
    except FileNotFoundError:
        abort_with_message(f"There are missing parents in path {result_directory}")
    else:
        print(f"Result directory {result_directory} was created")
    return result_directory
