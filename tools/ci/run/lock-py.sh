#!/bin/bash

set -e

source .venv/bin/activate
pdm lock --no-default -d -p tools/ci
