#!/bin/bash

set -e

source .venv/bin/activate
pylint --rcfile tools/ci/.pylintrc $@
