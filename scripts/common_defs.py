import logging
import sys
from pathlib import Path

LOGGER = logging.getLogger(__name__)
PARENT_DIRECTORY = Path(".").parent.parent


def critical_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)


def get_device_name():
    LOGGER.info("Getting device name")
    LOGGER.debug("\"cpuinfo\" import")
    try:
        import cpuinfo
    except ImportError:
        critical_message("ImportError while importing cpuinfo")
    arch = cpuinfo.get_cpu_info().get("arch")
    if arch == "X86_64":
        LOGGER.info("\"x86\" device name was chosen")
        return "x86"
    LOGGER.info("\"kendryte\" device name was chosen")
    return "kendryte"

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