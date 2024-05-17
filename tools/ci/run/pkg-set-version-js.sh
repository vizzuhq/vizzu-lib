#!/bin/bash

set -e

version=$(node ./test/e2e/test.cjs -v --vizzu /dist/vizzu.min.js|awk -F'-' '{print $1}')
echo "version: $version"
node ./tools/ci/version.cjs $version

sed -i  '/\#\# \[Unreleased\]/,/\#\#\ \[/{//!d}' CHANGELOG.md
sed -i '/\#\# \[Unreleased\]/d' CHANGELOG.md

npm version $version --no-git-tag-version
