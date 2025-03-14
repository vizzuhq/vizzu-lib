#!/bin/bash

set -e

version=$(node ./test/e2e/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}')
echo "version: $version"
npm cache clean --force && sleep 5
npm install vizzu@$version
node ./tools/ci/purge.cjs $version
