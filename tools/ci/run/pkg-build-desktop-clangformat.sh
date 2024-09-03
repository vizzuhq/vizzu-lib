#!/bin/bash

set -e

# build desktop

DEFAULT_JOBS=4
JOBS="$1"
if [ -z "$1" ]; then
    JOBS=$DEFAULT_JOBS
fi
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
mkdir -p build/cmake-desktop-clang
pushd build/cmake-desktop-clang
cmake -Dclangformat:BOOL="ON" ../../project/cmake/
cmake --build . -- --jobs=$JOBS
popd

# test desktop

pushd build/cmake-desktop-clang/test
./vizzutest
popd
