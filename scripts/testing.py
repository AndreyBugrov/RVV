import subprocess
import shlex
import logging
from pathlib import Path

from common_defs import abort_with_message
from compilation import get_binary_path


LOGGER = logging.getLogger(__name__)

TEST_COUNT = 96


def run_tests(bin_path: Path, show: bool) -> tuple[int, int]:
    args = f"{bin_path}"
    cmd = shlex.split(args)
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output = proc.communicate()
    if output[1]:
        stderr = output[1].decode("utf-8")
        abort_with_message(f"Errors in test:\n{stderr}") # can't get previous stdout if any error occured
    returncode = proc.returncode
    stdout = output[0].decode("utf-8")
    if show:
        test_output = stdout.split("FAILED TESTS: \n")[0]
        print(f"Test output:\n\n{test_output}")
    if returncode:
        if not stdout or not "FAILED TESTS:" in stdout:
            abort_with_message(f"Process returned non-zero code {returncode}")
        LOGGER.warning(f"{returncode} tests were failed")
        failed_tests = stdout.split("FAILED TESTS: \n")[1]
        LOGGER.warning(f"Failed tests: \n{failed_tests}")
    else:
        LOGGER.info("Tests passed")
    total_info = stdout.split("TOTAL")[1]
    passed_count = int(total_info.split("PASSED: ")[1].split('\n')[0])
    failed_count = int(total_info.split("FAILED: ")[1].split('\n')[0])
    return passed_count, failed_count


def full_test(compilation_profiles: list[str], device_name: str, no_recompile: bool, show: bool):
    all_passed_count = 0
    all_failed_count = 0
    failed_profiles = set()
    for compilation_profile in compilation_profiles:
        LOGGER.info(f"Current compilation profile: {compilation_profile}")
        bin_path = get_binary_path(compilation_profile, device_name, compilation_type="test", eigen_path=None, no_recompile=no_recompile)
        LOGGER.info(f"Running tests")
        current_passed_count, current_failed_count = run_tests(bin_path, show)
        all_failed_count += current_failed_count
        all_passed_count += current_passed_count
        if current_failed_count:
            failed_profiles.add(compilation_profile)
    LOGGER.info(f"All test count: {all_passed_count + all_failed_count}")
    LOGGER.info(f"Passed test count: {all_passed_count}")
    LOGGER.info(f"Failed test count: {all_failed_count}")
    if all_failed_count:
        failed_profiles_str = "\", \"".join(failed_profiles)
        LOGGER.info(f"Tests were failed in \"{failed_profiles_str}\" profiles")