#!/bin/bash

set -e

# build wasm

DEFAULT_JOBS=8
JOBS="$1"
if [ -z "$1" ]; then
    JOBS=$DEFAULT_JOBS
fi
mkdir -p build/cmake-wasm
pushd build/cmake-wasm
emcmake cmake ../../project/cmake/
cmake --build . -- --jobs=$JOBS
popd

# test wasm

pushd build/cmake-wasm/test
cp vizzutest.js vizzutest.cjs
node vizzutest.cjs
popd

# test wasm size

wget 'https://vizzu-lib-main.storage.googleapis.com/lib/size.txt' -O build/cmake-wasm/weblib/size_downloaded.txt
node tools/ci/sizeCheck.cjs

# build ts

npm run build:ts

# test d.ts

npm run type:src

# test js

npm run test:unit

# rollup js

npm run rollup
