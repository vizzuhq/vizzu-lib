#!/bin/bash

set -e

source .venv/bin/activate
python ./tools/ci/std_check.py mdformat --wrap 80 --end-of-line keep --line-length 70 $@
