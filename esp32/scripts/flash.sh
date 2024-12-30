#!/bin/bash

if [ "$#" -eq 0 ]; then
    echo "No flash arguments provided."
elif [ "$#" -ne 2 ]; then
    echo "Please provide the application and port to flash the device to"
else
	. ./esp-idf/export.sh
    export IDF_TOOLS_PATH="$HOME/miniconda3/envs/$CONDA_DEFAULT_ENV"
    cd build/$1
    python $IDF_PATH/components/esptool_py/esptool/esptool.py -p $2 write_flash @flash_project_args
    python -m esp_idf_monitor -p $2 bristlemouth_esp32.elf --decode-panic backtrace
fi
