import logging
import sys
from importlib import import_module
from pathlib import Path

LOGGER = logging.getLogger(__name__)
PARENT_DIRECTORY = Path(".").parent.parent

X86_NAME = "x86"
RISC_V_NAME = "risc_v"
VM_NAME = "vm"

TIME_NAME = "Time"
REF_TIME_NAME = "Eigen Time"
VEC_COLUMN_NAMES = ["Experiments count", "Length", TIME_NAME, REF_TIME_NAME]
MAT_COLUMN_NAMES = ["Experiments count", "1st row count", "1st column count", "2nd column count", TIME_NAME, REF_TIME_NAME]
GS_COLUMN_NAMES = ["Experiments count", "Vector system size", "Vector length", TIME_NAME, REF_TIME_NAME]
QR_COLUMN_NAMES = ["Experiments count", "Row count", "Column count", TIME_NAME, REF_TIME_NAME]


def abort_with_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)


def import_module_by_name(name: str):
    LOGGER.debug(f"\"{name}\" import")
    try:
        return import_module(name)
    except ImportError as e:
        abort_with_message(f"Import error while importing {name}: {e}")


def get_device_name() -> str:
    try:
        import_module("cpuinfo")
    except ModuleNotFoundError:
        return RISC_V_NAME
    cpuinfo = import_module_by_name("cpuinfo")
    arch = cpuinfo.get_cpu_info().get("arch")
    if arch == "X86_64":
        if Path('/sys/devices/system/cpu').is_dir():
            device_name = X86_NAME
        else:
            device_name = VM_NAME
    else:
        device_name = RISC_V_NAME
    return device_name


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


def do_not_setup_frequency(device_name):
    return device_name == VM_NAME
