import subprocess
import logging
from pathlib import Path
from datetime import datetime

from common_defs import critical_message

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


def set_min_core_frequency_limit(frequency, core_num):
    line = f"sudo sh -c 'echo {frequency} > /sys/devices/system/cpu/cpu{core_num}/cpufreq/scaling_min_freq'"
    subprocess.Popen(line, shell=True).communicate()


def _output_dir_exists(output_dir: str):
    output_dir_path = Path(output_dir)
    return output_dir_path.is_dir()


def prepare_result_directory(output_dir: str, suffix: str | None) -> Path:
    LOGGER.info("Check output directory")
    if not _output_dir_exists(output_dir):
        critical_message(f"Output directory {output_dir} does not exist")
    parent_directory = Path(output_dir)
    current_datetime = datetime.today().strftime('%Y%m%d_%H%M%S')
    result_directory = parent_directory / current_datetime
    if suffix:
        result_directory = Path(str(result_directory) + "_" + suffix)
    try:
        result_directory.mkdir(parents=False, exist_ok=False)
    except FileExistsError:
        critical_message(f"Directory {result_directory} already exists")
    LOGGER.info(f"Result directory {result_directory} was created")
    return result_directory
