#!/bin/bash

set -e

rm -rf build/js
mkdir -p build/js
npm pack --pack-destination build/js
tar -ztvf build/js/*.tgz
