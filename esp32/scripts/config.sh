#!/bin/bash
if [ "$#" -eq 0 ]; then
    echo "No build type arguments provided."
else
	. ./esp-idf/export.sh
    export IDF_TOOLS_PATH="$HOME/miniconda3/envs/$CONDA_DEFAULT_ENV"
    mkdir -p build/$1
    ninja -C build/$1 menuconfig
fi
