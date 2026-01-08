import subprocess
import shlex
import logging
from pathlib import Path

from common_defs import critical_message
from compilation import get_binary_path


LOGGER = logging.getLogger(__name__)

TEST_COUNT = 96


def run_tests(bin_path: Path):
    args = f"{bin_path}"
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = proc.communicate()
    if output[1]:
        stderr = output[1].decode("utf-8")
        critical_message(f"Errors in test:\n{stderr}") # can't get previous stdout if any error occured
    returncode = proc.returncode
    stdout = output[0].decode("utf-8")
    if returncode:
        LOGGER.warning(f"{returncode} tests were failed")
        test_output = stdout.split("FAILED TESTS: \n")[0]
        LOGGER.debug(f"Test output:\n{test_output}")
        failed_test = stdout.split("FAILED TESTS: \n")[1]
        LOGGER.warning(f"Failed tests: \n{failed_test}")
    else:
        LOGGER.info("Tests passed")
        LOGGER.debug(f"Test output:\n{stdout}")
    return returncode


def full_test(compilation_profiles: list[str], device_name: str, no_recompile: bool):
    all_test_count = TEST_COUNT * len(compilation_profiles)
    all_failed_test_count = 0
    failed_profiles = set()
    for compilation_profile in compilation_profiles:
        LOGGER.info(f"Compilation profile: {compilation_profile}")
        bin_path = get_binary_path(compilation_profile, device_name, compilation_type="test", eigen_path=None, no_recompile=no_recompile)
        LOGGER.info(f"Running tests")
        current_failed_test_count = run_tests(bin_path)
        all_failed_test_count += current_failed_test_count
        if current_failed_test_count:
            failed_profiles.add(compilation_profile)
    LOGGER.info(f"All test count: {all_test_count}")
    LOGGER.info(f"Passed test count: {all_test_count-all_failed_test_count}")
    LOGGER.info(f"Failed test count: {all_failed_test_count}")
    if all_failed_test_count:
        failed_profiles_str = "\", \"".join(failed_profiles)
        LOGGER.info(f"Tests were failed in \"{failed_profiles_str}\" profiles")