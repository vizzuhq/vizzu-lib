#!/bin/bash

set -e

rm -rf build/js
mkdir -p build/js
version="$1"
if [ -z "$version" ]; then
  version=$(node ./test/integration/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}')
  echo "version: $version"
fi
npm version $version --no-git-tag-version
npm pack --pack-destination build/js
tar -ztvf build/js/*.tgz
