import logging
import sys

LOGGER = logging.getLogger(__name__)

def critical_message(msg: str):
    LOGGER.critical(f"{msg}")
    sys.exit(-1)
