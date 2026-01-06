import logging
import sys
from pathlib import Path

LOGGER = logging.getLogger(__name__)
PARENT_DIRECTORY = Path(".").parent.parent

X86_NAME = "x86"
RISC_NAME = "risc_v"

def critical_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)


def get_device_name() -> str:
    LOGGER.info("Getting device name")
    try:
        LOGGER.debug("\"cpuinfo\" import")
        import cpuinfo
    except ImportError:
        critical_message("ImportError while importing cpuinfo")
    device_name = RISC_NAME
    arch = cpuinfo.get_cpu_info().get("arch")
    if arch == "X86_64":
        device_name = X86_NAME
    LOGGER.info(f"\"{device_name}\" device name was chosen")
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
