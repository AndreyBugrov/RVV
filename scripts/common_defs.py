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