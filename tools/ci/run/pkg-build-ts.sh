#!/bin/bash

set -e

node tools/ci/type/gen-presets.cjs
node tools/ci/type/gen-dts.cjs
node tools/ci/type/gen-canvas-ts.cjs
npx tsc -p src/apps/weblib/ts-api/tsconfig.json
