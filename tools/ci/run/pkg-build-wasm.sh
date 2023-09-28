#!/bin/bash

set -e

# build wasm

mkdir -p build/cmake-wasm
pushd build/cmake-wasm
emcmake cmake ../../project/cmake/
cmake --build . -- --jobs=8
popd

# test wasm

pushd build/cmake-wasm/test
cp vizzutest.js vizzutest.cjs
node vizzutest.cjs
popd

# test wasm size

wget 'https://vizzu-lib-main.storage.googleapis.com/lib/size.txt' -O build/cmake-wasm/weblib/size_downloaded.txt
python tools/ci/size_check.py

# test js

npm run test-unit-src

# rollup js

./tools/ci/run/pkg-rollup-js.sh

# test d.ts

npm run type-src-js
