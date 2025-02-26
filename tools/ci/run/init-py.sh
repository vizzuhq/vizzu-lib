#!/bin/bash

set -e

if command -v python3.13 &>/dev/null; then
    PYTHON=python3.13
else
    PYTHON=python3
    if ! $PYTHON -c 'import sys; assert sys.version_info >= (3,13)' > /dev/null 2>&1; then
        echo "Python 3.13+ is required"
        exit 1
    fi
fi

test -f ~/.netrc && chmod u+rw,u-x,go-rwx ~/.netrc

$PYTHON -m venv --copies ".venv"
source .venv/bin/activate
pip install pdm==2.22.3
pdm install -p tools/ci --no-isolation
