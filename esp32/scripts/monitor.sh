#!/usr/bin/env bash
if [ "$#" -eq 0 ]; then
    echo "No flash arguments provided."
elif [ "$#" -ne 2 ]; then
    echo "Please provide the application and port to monitor the device to"
else
	. ./esp-idf/export.sh
    export IDF_TOOLS_PATH="$HOME/miniconda3/envs/$CONDA_DEFAULT_ENV"
    cd build/$1
    python -m esp_idf_monitor -p $2 bristlemouth_esp32.elf --decode-panic backtrace
fi
