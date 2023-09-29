#!/bin/bash

set -e

# build desktop

export CC=/usr/bin/gcc-12
export CXX=/usr/bin/g++-12
mkdir -p build/cmake-desktop-gcc
pushd build/cmake-desktop-gcc
cmake -Dclangformat:BOOL="ON" ../../project/cmake/
cmake --build . -- --jobs=4
popd

# test desktop

pushd build/cmake-desktop-gcc/test
./vizzutest
popd
