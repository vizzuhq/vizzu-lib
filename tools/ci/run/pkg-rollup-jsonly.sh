#!/bin/bash

set -e

rm -rf dist
mkdir -p dist

cp src/apps/weblib/js-api/* dist/

LIB_URL="https://vizzu-lib-main.storage.googleapis.com/lib"
if [ "$1" ]; then
  LIB_URL="https://vizzu-lib-main-sha.storage.googleapis.com/lib-$1"
fi

wget "$LIB_URL/cvizzu.js" -O dist/cvizzu.js
wget "$LIB_URL/cvizzu.wasm" -O dist/cvizzu.wasm

npm run pkg-rollup-js
