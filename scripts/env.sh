#! /usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source "${SCRIPT_DIR}/plots/bin/activate"

sudo su <<sudo_end # https://stackoverflow.com/questions/24640340/why-cant-i-use-sudo-su-within-a-shell-script-how-to-make-a-shell-script-run
echo -1 > /proc/sys/kernel/perf_event_paranoid
echo 0 > /proc/sys/kernel/kptr_restrict
sudo_end
