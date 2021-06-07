#!/bin/bash
make clean
make 2>&1 | grep -o '\[-W.*\]' | perl -ne 'print unless $seen{$_}++'
