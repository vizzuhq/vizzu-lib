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
cmake -Dclangtidy:BOOL="ON" -Dcppcheck:BOOL="ON" ../../project/cmake/
cmake --build . --target vizzutest -- --jobs=$JOBS
popd

# test desktop

pushd build/cmake-desktop/test
./vizzutest
popd
