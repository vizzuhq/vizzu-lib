#!/bin/bash

set -e

# build wasm

mkdir -p /workspace/build/cmake-wasm
pushd /workspace/build/cmake-wasm
emcmake cmake ../../project/cmake/
cmake --build . -- --jobs=8
popd

# test wasm

pushd /workspace/build/cmake-wasm/test
cp vizzutest.js vizzutest.cjs
node vizzutest.cjs
popd

# test js

pushd /workspace
npm run test-unit-src
popd

# rollup js

pushd /workspace
./tools/ci/run/pkg-rollup.sh
popd

# test d.ts

pushd /workspace
npm run type-src
popd
