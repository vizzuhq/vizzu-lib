#!/bin/bash

set -e

node tools/ci/type/gen-presets.cjs
node tools/ci/type/gen-dts.cjs
node tools/ci/type/gen-canvas-ts.cjs
npx tsc -p src/apps/weblib/ts-api/tsconfig.json
find "./src/apps/weblib/ts-api/" -type f -name "*.d.ts" -exec bash -c '
    for file do
        relative_path=$(realpath --relative-to="./src/apps/weblib/ts-api/" "$file")
        destination_path="./dist/$relative_path"
        mkdir -p "$(dirname "$destination_path")"
        cp "$file" "$destination_path"
    done
' bash {} +
