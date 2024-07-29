#!/bin/bash

set -e

source .venv/bin/activate
pdm lock --no-default --no-isolation -d -p tools/ci
