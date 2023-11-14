#!/bin/bash

set -e

if ! python3 -c 'import sys; assert sys.version_info >= (3,10)' > /dev/null; then
    echo "Python 3.10+ is required"
    exit 1
fi

python3 -m venv --copies ".venv"
source .venv/bin/activate
pip install pdm==2.8.0
pdm install -p tools/ci
