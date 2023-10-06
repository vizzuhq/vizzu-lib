#!/bin/bash

set -e

source .venv/bin/activate

python tools/docs/examples/gen_thumbnails.py
