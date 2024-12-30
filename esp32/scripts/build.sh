#!/bin/bash
if [ "$#" -eq 0 ]; then
    echo "No build type arguments provided."
else
    export IDF_TOOLS_PATH="$HOME/miniconda3/envs/$CONDA_DEFAULT_ENV"
	. ./esp-idf/export.sh
    mkdir -p build/$1
    cd build/$1
    cmake ../.. -DCMAKE_TOOLCHAIN_FILE=$IDF_PATH/tools/cmake/toolchain-esp32.cmake -DTARGET=esp32s3 -GNinja -DBUILD_TYPE=$1
    cmake --build .
    cd ..
fi
