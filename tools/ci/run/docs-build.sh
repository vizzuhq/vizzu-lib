#!/bin/bash

set -e

git clone --single-branch --branch main --depth 1 'git@github.com:vizzuhq/vizzu-lib-doc'

rm -rf docs/assets/dist
mkdir -p docs/assets/dist
cp dist/vizzu.min.js docs/assets/dist/vizzu.min.js
cp dist/cvizzu.wasm docs/assets/dist/cvizzu.wasm

rm -rf docs/showcases
cp -r vizzu-lib-doc/docs/showcases docs/
rm -rf docs/readme
cp -r vizzu-lib-doc/docs/readme docs/

source .venv/bin/activate
mkdocs build -f ./tools/docs/mkdocs.yml
