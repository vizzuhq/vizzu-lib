#!/bin/bash
make clean
make 2>&1 | grep 'warning:' | grep -o '\[.*\]' | perl -ne 'print unless $seen{$_}++'
