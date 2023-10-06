#!/bin/bash

set -e

hash=$(node ./tools/docs/examples/gen_webcontent_hash.cjs)
echo $hash

if gsutil -q stat "gs://vizzu-lib-thumbnails/$hash/exists.txt"; then
    gsutil -m cp -r "gs://vizzu-lib-thumbnails/$hash/analytical_operations" /workspace/docs/examples/
    gsutil -m cp -r "gs://vizzu-lib-thumbnails/$hash/presets" /workspace/docs/examples/
    gsutil -m cp -r "gs://vizzu-lib-thumbnails/$hash/static" /workspace/docs/examples/
else
    ./tools/ci/run/docs-gen-thumbnail.sh
    gsutil -m cp -r /workspace/docs/examples/analytical_operations "gs://vizzu-lib-thumbnails/$hash/"
    gsutil -m cp -r /workspace/docs/examples/presets "gs://vizzu-lib-thumbnails/$hash/"
    gsutil -m cp -r /workspace/docs/examples/static "gs://vizzu-lib-thumbnails/$hash/"
    echo "exists" | gsutil cp - gs://vizzu-lib-thumbnails/$hash/exists.txt
fi
