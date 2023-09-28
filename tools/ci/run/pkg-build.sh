#!/bin/bash

set -e

./tools/ci/run/pkg-rollup.sh

rm -rf build/js
mkdir -p build/js
npm version $(node test/integration/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}') --no-git-tag-version
npm pack --pack-destination build/js
tar -ztvf build/js/*.tgz
