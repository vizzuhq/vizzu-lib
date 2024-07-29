#!/bin/bash

set -e

if ! python3 -c 'import sys; assert sys.version_info >= (3,10)' > /dev/null; then
    echo "Python 3.10+ is required"
    exit 1
fi

test -f ~/.netrc && chmod u+rw,u-x,go-rwx ~/.netrc

python3.10 -m venv --copies ".venv" || python3 -m venv --copies ".venv"
source .venv/bin/activate
pip3 install "setuptools<72.0.0"
pip install pdm==2.10.3
pdm install -p tools/ci
