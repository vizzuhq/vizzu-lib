#!/bin/bash

set -e

source .venv/bin/activate
black $@
