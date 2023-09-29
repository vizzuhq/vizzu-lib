#!/bin/bash

set -e

# build desktop

mkdir -p build/cmake-desktop
pushd build/cmake-desktop
cmake -Dclangtidy:BOOL="ON" -Dcppcheck:BOOL="ON" ../../project/cmake/
cmake --build . --target cvizzu vizzutest -- --jobs=8
popd

# test desktop

pushd build/cmake-desktop/test
./vizzutest
popd
