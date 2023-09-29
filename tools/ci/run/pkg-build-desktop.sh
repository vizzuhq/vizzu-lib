#!/bin/bash

set -e

# build desktop

mkdir -p build/cmake-desktop
pushd build/cmake-desktop
cmake ../../project/cmake/
cmake --build . -- --jobs=8
popd

# test desktop

pushd build/cmake-desktop/test
./vizzutest
popd
