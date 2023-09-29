#!/bin/bash

set -e

python3.10 -m venv --copies ".venv"
source .venv/bin/activate
pip install pdm==2.8.0
pdm install
