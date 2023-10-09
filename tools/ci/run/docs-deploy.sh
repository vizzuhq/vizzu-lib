#!/bin/bash

set -e

source .venv/bin/activate
python tools/docs/deploy.py
