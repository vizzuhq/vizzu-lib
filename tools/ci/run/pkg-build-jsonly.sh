#!/bin/bash

set -e

rm -rf dist
mkdir -p dist

cp src/apps/weblib/js-api/* dist/

wget 'https://vizzu-lib-main.storage.googleapis.com/lib/cvizzu.js' -O dist/cvizzu.js
wget 'https://vizzu-lib-main.storage.googleapis.com/lib/cvizzu.wasm' -O dist/cvizzu.wasm

npm run pkg-build
