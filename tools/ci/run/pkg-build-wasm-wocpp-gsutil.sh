#!/bin/bash

set -e

rm -rf dist
mkdir -p dist

SHORT_SHA="$1"
gsutil cp "gs://vizzu-lib-main-sha/lib-$SHORT_SHA/cvizzu.wasm" 'dist/cvizzu.wasm'
gsutil cp "gs://vizzu-lib-main-sha/lib-$SHORT_SHA/cvizzu.js" 'dist/cvizzu.js'

npm run pkg-build-ts
npm run pkg-rollup-js
