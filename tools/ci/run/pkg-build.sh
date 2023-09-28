#!/bin/bash

set -e

rm -f dist/*.d.ts dist/*.min.js
npm run type-gen
npx rollup --config project/js/rollup.config.cjs

rm -rf build/js
mkdir -p build/js
npm version $(node test/integration/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}')
npm pack --pack-destination build/js
tar -ztvf build/js/*.tgz
