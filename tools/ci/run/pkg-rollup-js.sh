#!/bin/bash

set -e

rm -f dist/*.min.js
npx rollup --config tools/ci/rollup.config.cjs
