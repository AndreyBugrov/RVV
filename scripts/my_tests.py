import subprocess
import shlex
import logging
from pathlib import Path

from common import critical_message


LOGGER = logging.getLogger(__name__)

def run_tests(bin_path: Path):
    args = f"{bin_path}"
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = proc.communicate()
    if output[1]:
        stderr = output[1].decode("utf-8")
        critical_message(f"Errors in test:\n{stderr}")
    returncode = proc.returncode
    stdout = output[0].decode("utf-8")
    if returncode:
        LOGGER.warning(f"{returncode} tests were failed")
        test_output = stdout.split("FAILED TESTS: \n")[0]
        LOGGER.debug(f"Test output:\n{test_output}")
        failed_test = stdout.split("FAILED TESTS: \n")[1]
        LOGGER.warning(f"FAILED TESTS: \n{failed_test}")
    else:
        LOGGER.info("Tests were passed")
        LOGGER.debug(f"Test output:\n{stdout}")
    return returncode
 