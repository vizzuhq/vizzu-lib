#!/bin/bash

set -e

rm -f dist/*.d.ts dist/*.min.js
npm run type-gen
npx rollup --config tools/ci/rollup.config.cjs
