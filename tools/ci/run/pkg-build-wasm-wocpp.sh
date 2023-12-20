#!/bin/bash

set -e

rm -rf dist
mkdir -p dist

LIB_URL="https://vizzu-lib-main.storage.googleapis.com/lib"
if [ "$1" ]; then
  LIB_URL="https://vizzu-lib-main-sha.storage.googleapis.com/lib-$1"
fi

MAX_RETRIES=10
RETRY_DELAY=5
SUCCESS=false
for i in $(seq 1 $MAX_RETRIES); do
  if wget "$LIB_URL/cvizzu.js" -O "dist/cvizzu.js" && wget "$LIB_URL/cvizzu.wasm" -O "dist/cvizzu.wasm"; then
    echo "Download successful"
    SUCCESS=true
    break
  else
    echo "Download failed (Attempt $i/$MAX_RETRIES). Retrying in $RETRY_DELAY seconds..."
    sleep $RETRY_DELAY
  fi
done
if [ "$SUCCESS" = false ]; then
    echo "All download attempts failed. Exiting with a non-zero status code."
    exit 1
fi

npm run build:ts
npm run rollup
