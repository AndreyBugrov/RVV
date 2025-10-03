import logging
import subprocess
import shlex

from compilation import compile_sources


LOGGER = logging.getLogger(__name__)

# def get_disasm(compilation_profiles, device_name, output_dir):
#     for compilation_profile in compilation_profiles:
#         bin_path = compile_sources(compilation_profile, device_name, is_test=False, for_perf=True)
#         args = f"objdump -S -d {bin_path} > "
#         perf_object_name = "Perf record"
#         LOGGER.debug(f"{perf_object_name} line: {args}")
#         cmd = shlex.split(args)
#         LOGGER.info(f"objdump launching")
#         proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
#         LOGGER.debug(f"{perf_object_name} trace was saved to {perf_data_path}")
#         output = proc.communicate()
#         stderr = output[1]
#         if stderr:
#             perf_record_info = stderr.decode('utf-8')
#             LOGGER.warning(f"{perf_object_name} output:\n{perf_record_info}")
#         if proc.returncode:
#             terminate_experiment(f"Process has completed with non-zero return code: {proc.returncode}")