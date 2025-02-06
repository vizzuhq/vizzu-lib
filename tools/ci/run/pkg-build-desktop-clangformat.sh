#!/bin/bash

set -e

# build desktop

DEFAULT_JOBS=4
JOBS="$1"
if [ -z "$1" ]; then
    JOBS=$DEFAULT_JOBS
fi
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
mkdir -p build/cmake-desktop-gcc
pushd build/cmake-desktop-gcc
cmake -Dclangformat:BOOL="ON" ../../project/cmake/
cmake --build . -- --jobs=$JOBS
popd

# test desktop

pushd build/cmake-desktop-gcc/test
./vizzutest
popd
