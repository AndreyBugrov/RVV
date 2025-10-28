#! /usr/bin/bash

BASE_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[1]}" )" &> /dev/null && pwd )
VENV_DIR="${BASE_DIR}/venv"
if [ ! -d "${VENV_DIR}" ]; then
    mkdir "${VENV_DIR}"
    python3 -m venv "${VENV_DIR}"
    source "${VENV_DIR}/bin/activate"
    pip install -r "${BASE_DIR}/scripts/requirements.txt"
else
    source "${VENV_DIR}/bin/activate"
fi
export CCACHE_DIR=${BASE_DIR}/cache_dir