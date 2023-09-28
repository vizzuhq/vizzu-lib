#!/bin/bash

set -e

source .venv/bin/activate
pylint $@
