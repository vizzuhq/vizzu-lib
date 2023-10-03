#!/bin/bash

set -e

# build desktop

DEFAULT_JOBS=8
JOBS="$1"
if [ -z "$1" ]; then
    JOBS=$DEFAULT_JOBS
fi
mkdir -p build/cmake-desktop
pushd build/cmake-desktop
cmake ../../project/cmake/
cmake --build . -- --jobs=$JOBS
popd

# test desktop

pushd build/cmake-desktop/test
./vizzutest
popd
