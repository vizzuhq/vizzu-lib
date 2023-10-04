#!/bin/bash

set -e

node ./tools/ci/version.cjs $(node ./test/integration/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}')

sed -i  '/\#\# \[Unreleased\]/,/\#\#\ \[/{//!d}' CHANGELOG.md
sed -i '/\#\# \[Unreleased\]/d' CHANGELOG.md

npm run pkg-build-js
