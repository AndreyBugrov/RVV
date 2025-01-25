import logging
import sys
from pathlib import Path

LOGGER = logging.getLogger(__name__)
PARENT_DIRECTORY = Path(".").parent.parent


def critical_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)
